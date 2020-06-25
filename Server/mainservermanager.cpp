#include "mainservermanager.h"
#include "userstatistics.h"
using namespace Mafia;


MainServerManager::MainServerManager(int argc, char* argv[], QObject* parent) : QObject(parent)
{
    for(int i = 1; i < argc; i++){
        std::cout << argv[i] << std::endl;
    }

    try {
        dbWorker = new Database::DatabaseWorker();

        usersDb = new Database::UserDatabaseManager(dbWorker);

        gamesDb = new Database::GameDatabaseManager(dbWorker);

        chatSettingsDb = new Database::ChatSettingsDatabaseManager(dbWorker);

        chatsDb = new Database::ChatDatabaseManager(dbWorker);

        transactionsDb = new Database::TransactionDatabaseManager(dbWorker);


        _networker_test();
        //_database_test();
        //_database_get_all();
    } catch (Exceptions::Exception* exception) {
        exception->show();
    }

}

void MainServerManager::on_message_received(Message message)
{

}

void MainServerManager::create_user(QString nickname, QString email, QString password, Client client, Network::MessageIdType requestId)
{
    try {
        Database::UserIdType newUserId = usersDb->add_user(nickname, email, password);
        if(clients.contains(client)){
            int index = clients.indexOf(client);
            if(users[index] != nullUser){
                usersDb->logout_user(users[index]);
                networker->send_message(Message(MessageType_LoggedOut, (SymbolType*)"", 1, client));
            }
            users[index] = newUserId;
        } else{
            clients.append(client);
            users.append(newUserId);
        }

        char* messageData = new SymbolType[sizeof(Database::UserIdType) + email.length()];

        for(unsigned int i = 0; i < sizeof (Database::UserIdType); i++){
            messageData[i] = ((char*)&newUserId)[i];
        }

        std::string data = email.toStdString();

        for(int i = 0; i < email.length(); i++){
            messageData[i + sizeof (Database::UserIdType)] = data[i];
        }

        networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)messageData, sizeof(Database::UserIdType) + email.length(),
                                        client, requestId));
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            networker->send_message(Message(MessageType_UnableToCreateUser, (SymbolType*)"", 1, client));
            break;
        }
        default:{
            exception->show();
            break;
        }
        }
    }
}

void MainServerManager::send_chat_message(Database::UserIdType sender, Database::ChatIdType toChat, QString data,
                                          QList<Database::MessageIdType> answerFor, Database::ChatFeature feature)
{
    try {
        Database::ChatMessage message = Database::ChatMessage();
        message.data = data;
        message.from = sender;
        message.toChat = toChat;
        message.feature = feature;
        message.answerFor = answerFor;

        if(chatSettingsDb->can_send_message(sender, toChat)){
            chatsDb->add_message(message);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"", 1, get_client_by_user(sender)));
        }

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::login_user(QString email, QString password, Client client, Network::MessageIdType requestId)
{
    try {
        Database::UserIdType userId = usersDb->get_id(email);
        bool login = usersDb->login_user(userId, password);
        if(login){
            if(clients.contains(client)){
                users[clients.indexOf(client)] = userId;
            } else{
                clients.append(client);
                users.append(userId);
            }
        } else{
            networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)"Invalid password or email", 26, client, requestId));
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::create_chat(Database::UserIdType creator, Network::MessageIdType requestId)
{
    try {
        Database::ChatIdType newId = chatSettingsDb->create_chat();

        chatSettingsDb->add_user_to_chat(creator, newId, Database::ChatCapabilities_Admin);

        networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)&newId, sizeof (newId), get_client_by_user(creator), requestId));
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }

}

void MainServerManager::get_last_messages(Database::UserIdType user, Database::ChatIdType chat, Network::MessageIdType requestId, int messagesCount)
{
    try {
        bool canGet = chatSettingsDb->can_read_message(user, chat);

        if(canGet){
            MafiaList<Database::ChatMessage> allMessages;
            if(messagesCount == -1){
                allMessages = chatsDb->get_last_messages(chat);
            } else{
                allMessages = chatsDb->get_last_messages(chat, messagesCount);
            }

            std::string data = System::Serializer::serialize<MafiaList<Database::ChatMessage>>(allMessages);

            networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)data.c_str(), data.length(),
                                            get_client_by_user(user), requestId));
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't read messages from this chat", 39,
                                            get_client_by_user(user)));
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::get_users_chats(Database::UserIdType user, Network::MessageIdType requestId, int chatsCount)
{
    MafiaList<Database::ChatIdType> usersChats = chatSettingsDb->get_chats_with(MafiaList<Database::ChatIdType>(),
                                                                          MafiaList<Database::UserIdType>() << user);

    MafiaList<Database::ChatMessage> messages = chatsDb->get_messages(usersChats);

    MafiaList<Database::ChatIdType> chats = MafiaList<Database::ChatIdType>();
    int index = 0;
    while(chats.length() < usersChats.length() && chats.length() < chatsCount && index < messages.length()){
        if(!chats.contains(messages[index].toChat)){
            chats.append(messages[index].toChat);
        }
        index++;
    }
    int i = 0;
    while(chats.length() < usersChats.length() && chats.length() < chatsCount){
        if(!chats.contains(usersChats[i])){
            chats.append(usersChats[i]);
        }
        i++;
    }

    std::string data = System::Serializer::serialize<MafiaList<Database::ChatIdType>>(chats);

    networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)data.c_str(), data.length(), get_client_by_user(user), requestId));
}

void MainServerManager::add_user_to_chat(Database::ChatIdType chat, Database::UserIdType user, Database::UserIdType initializer,
                                         Database::ChatCapability capability)
{
    Client asker = get_client_by_user(initializer);
    try{

        bool ableToEdit = chatSettingsDb->can_edit_users(initializer, chat);
        if(ableToEdit){
            chatSettingsDb->add_user_to_chat(user, chat, capability);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't edit users in this chat", 33, asker));
        }
    }
    catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt:{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"User is already in chat", 23, asker));
            break;
        }
        default:{
            throw exception;
            break;
        }
        }
    }
}

void MainServerManager::remove_user_from_chat(Database::ChatIdType chat, Database::UserIdType user, Database::UserIdType initializer)
{
    Client asker = get_client_by_user(initializer);
    try{

        bool ableToEdit = chatSettingsDb->can_edit_users(initializer, chat);
        if(ableToEdit){
            chatSettingsDb->remove_user_from_chat(user, chat);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't edit users in this chat", 33, asker));
        }
    }
    catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_DeleteMissedItemAttempt:{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"User is not in chat", 23, asker));
            break;
        }
        default:{
            throw exception;
            break;
        }
        }
    }
}

void MainServerManager::change_users_chat_capability(Database::ChatIdType chat, Database::UserIdType user, Database::ChatCapability newCapability,
                                                     Database::UserIdType initializer)
{
    Client asker = get_client_by_user(initializer);
    try{

        bool ableToEdit = chatSettingsDb->can_edit_users(initializer, chat);
        if(ableToEdit){
            chatSettingsDb->set_capability(user, chat, newCapability);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't edit users in this chat", 33, asker));
        }
    }
    catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt:{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"User is already in chat", 23, asker));
            break;
        }
        default:{
            throw exception;
            break;
        }
        }
    }
}

void MainServerManager::create_game(Client creator)
{

}

void MainServerManager::get_statistics(Database::UserIdType user, Database::UserIdType asker, Network::MessageIdType requestId)
{
    try {
        if(asker == user){

        }
        Database::User currentUser = usersDb->get_user(user);

        UserStatistics statistics = UserStatistics(currentUser);

        std::string data = System::Serializer::serialize<UserStatistics>(statistics);

        networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)data.c_str(), data.length(), get_client_by_user(asker), requestId));
    } catch (Exceptions::Exception* exception) {
        switch(exception->get_id()){
        default:{
            throw exception;
        }
        }
    }

}

void MainServerManager::add_game(Gameplay::Game game)
{
    try {
        Database::GameIdType current = gamesDb->add_game(game);
        game.id = current;
        usersDb->register_game(game);
    } catch (Exceptions::Exception* exception) {
        switch(exception->get_id()){
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::logout_user(Database::UserIdType user)
{
    try {
        usersDb->logout_user(user);
    } catch (Exceptions::Exception* exception) {
        switch(exception->get_id()){
        default:{
            throw exception;
        }
        }
    }
    if(users.contains(user)){
        users[users.indexOf(user)] = nullUser;
    }
}

void MainServerManager::get_logs_data(QString data, Client sender)
{

}

void MainServerManager::add_transaction(Database::Transaction transaction)
{
    try {
        Database::TransactionIdType id = transactionsDb->add_transaction(transaction.buyer, transaction.price, transaction.type);
        usersDb->add_transaction(transaction.buyer, id);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }

    switch (transaction.type) {
    case Database::TransactionType_BuySomething:{
        usersDb->set_account_type(transaction.buyer, Database::AccountType_Pro);
        break;
    }
    default:{
        throw new Exceptions::MainServerException(System::String("Unknown transaction type!"),
                                                  Exceptions::MainServerExceptionId_UnknownTransactionType);
    }
    }

}

void MainServerManager::change_nickname(Database::UserIdType user, QString newNickname)
{
    try {
        usersDb->change_nickname(user, newNickname);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            break;
        }
        }
    }
}

void MainServerManager::change_email(Database::UserIdType user, QString newEmail, Network::MessageIdType requestId)
{
    try {
        bool success = usersDb->change_email(user, newEmail);

        networker->send_message(Message(MessageType_RequestAnswer, (SymbolType*)System::Serializer::serialize<bool>(success).c_str(),
                                        (int)sizeof(bool), get_client_by_user(user), requestId));
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::change_achievement(Database::UserIdType user, Database::Achievement achievement)
{
    try {
        usersDb->add_achievement(user, achievement);
        Client client = get_client_by_user(user);
        networker->send_message(Message(MessageType_AchievementChange, (SymbolType*)&achievement, sizeof(achievement),
                                        client));

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::add_user_to_game(Client client, int gameId)
{

}

void MainServerManager::delete_message(Database::UserIdType user, Database::MessageIdType message)
{
    try {
        Database::ChatMessage messageData = chatsDb->get_message(message);
        bool canEdit = false;
        if(user == messageData.from){
            canEdit = chatSettingsDb->can_send_message(user, messageData.toChat);
        } else{
            canEdit = chatSettingsDb->can_edit_messages(user, messageData.toChat);
        }
        if(canEdit){
            chatsDb->delete_message(message);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't edit this message", 29, get_client_by_user(user)));
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::edit_message(Database::UserIdType user, Database::ChatMessage message)
{
    try {
        bool canEdit = false;
        if(user == message.from){
            canEdit = chatSettingsDb->can_send_message(user, message.toChat);
        } else{
            canEdit = chatSettingsDb->can_edit_messages(user, message.toChat);
        }
        if(canEdit){
            chatsDb->edit_message(message);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't edit this message", 29, get_client_by_user(user)));
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::read_message(Database::UserIdType user, Database::MessageIdType message)
{
    try {
        Database::ChatMessage messageData = chatsDb->get_message(message);
        bool canEdit = chatSettingsDb->can_read_message(user, messageData.toChat);

        if(canEdit){
            chatsDb->message_read(message, user);
        } else{
            networker->send_message(Message(MessageType_AccessDenied, (SymbolType*)"You can't edit this message", 29, get_client_by_user(user)));
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void MainServerManager::_get_data_from_request(Requests::NetworkRequest *req)
{
    if(req != 0){
        try {
            req->sleep_untill_finished();
            std::cout << "The answer is " << req->get_result<int>() << std::endl;
        } catch (Exception* exception) {
            exception->show();
        }
    }
}

void MainServerManager::_database_test()
{
    try {

        Database::UserIdType uid1 = usersDb->add_user("Nikita", "andrusov.n@gmail.com", "password1");
        Database::UserIdType uid2 = usersDb->add_user("SomeUser", "test@test.com", "testPassword");

        Database::ChatIdType chat = chatSettingsDb->create_chat();
        chatSettingsDb->add_user_to_chat(uid1, chat, Database::ChatCapabilities_Admin);
        chatSettingsDb->add_user_to_chat(uid2, chat);
        usersDb->add_user_to_chat(uid1, chat);
        usersDb->add_user_to_chat(uid2, chat);

        chatSettingsDb->create_chat();

        Database::TransactionIdType trid = transactionsDb->add_transaction(uid1, 999, Database::TransactionType_BuySomething);
        usersDb->add_transaction(uid1, trid);

        Database::TransactionIdType trid1 = transactionsDb->add_transaction(uid2, 1, Database::TransactionType_BuySomething);
        usersDb->add_transaction(uid2, trid1);

        Gameplay::Game game = Gameplay::Game();
        game.roles = MafiaList<Gameplay::Role>() << Gameplay::Role_Mafia << Gameplay::Role_Doctor;
        game.users = MafiaList<Database::UserIdType>() << uid1 << uid2;
        game.result = Gameplay::GameResult_MafiaWins;
        game.beginningDT = QDateTime::currentDateTimeUtc();
        game.endingDT = QDateTime::currentDateTimeUtc();

        game.id = gamesDb->add_game(game);


        usersDb->register_game(game);

        Gameplay::Game sgame = Gameplay::Game();
        sgame.roles = MafiaList<Gameplay::Role>() << Gameplay::Role_Mafia << Gameplay::Role_Doctor;
        sgame.users = MafiaList<Database::UserIdType>() << uid1 << uid2;
        sgame.result = Gameplay::GameResult_CiviliansWin;
        sgame.beginningDT = QDateTime::currentDateTimeUtc();
        sgame.endingDT = QDateTime::currentDateTimeUtc();

        sgame.id = gamesDb->add_game(sgame);
        usersDb->register_game(sgame);

        Database::ChatMessage chatMessage = Database::ChatMessage();

        chatMessage.data = "Hello world!";
        chatMessage.from = uid1;
        chatMessage.toChat = chat;
        chatMessage.feature = Database::ChatFeature_None;
        chatMessage.answerFor = MafiaList<Database::MessageIdType>();

        chatMessage.id = chatsDb->add_message(chatMessage);

        Database::ChatMessage chatMessage1 = Database::ChatMessage();

        chatMessage1.data = "Hi!";
        chatMessage1.from = uid2;
        chatMessage1.toChat = chat;
        chatMessage1.feature = Database::ChatFeature_None;
        chatMessage1.answerFor = MafiaList<Database::MessageIdType>() << chatMessage.id;

        chatMessage1.id = chatsDb->add_message(chatMessage1);

        chatMessage1.data = "Hello";
        chatMessage1.feature = Database::ChatFeature_Edited;

        chatsDb->edit_message(chatMessage1);

        _database_get_all();
    } catch (Exceptions::Exception* exception) {
        exception->show();
    }
}

void MainServerManager::_database_get_all()
{
    std::cout << "DATABASE DATA\n";

    MafiaList<Database::User> allUsers = usersDb->get_users();

    for(int i = 0; i < allUsers.length(); i++){
        allUsers[i].show();
    }

    MafiaList<Database::Transaction> allTransactions = transactionsDb->get_all_transactions();

    for(int i = 0; i < allTransactions.length(); i++){
        allTransactions[i].show();
    }

    MafiaList<Database::Chat> allChats = chatSettingsDb->get_all_chats();

    for(int i = 0; i < allChats.length(); i++){
        allChats[i].show();
    }

    MafiaList<Gameplay::Game> allGames = gamesDb->get_games_with();

    for(int i = 0; i < allGames.length(); i++){
        allGames[i].show();
    }

    MafiaList<Database::ChatMessage> allMessages = chatsDb->get_messages();

    for(int i = 0; i < allMessages.length(); i++){
        allMessages[i].show();
    }

    std::cout << "END DATABASE DATA\n";
}

void MainServerManager::_networker_test()
{
    networker = new MainServerNetworker(10000);
    Crypto::setKey("HaHA_UnDeCrYptAbLe_keY");
    Requests::NetworkRequest * myTestRequest = 0;
    try {
        myTestRequest = new Requests::NetworkRequest(networker,
                                              Message(MessageType_AbstractRequest,
                                                      (char*)"To be or not to be... That is the question!",
                                                      44,
                                                Client(QHostAddress("192.168.1.66").toIPv4Address(),
                                                       10000)));



    } catch (Exception* exception) {
        exception->show();
    }

    Subservers::SubserverObject* subserver = new Subservers::SubserverObject(networker, 100001,
                        System::String("D:\\Dropbox\\Dropbox\\Nikita\\Programs_1\\c++\\Mafia\\ClientSumm\\ClientSumm\\Client-summ\\release\\ForLibsTest.exe"),
                        System::String("ForLibsTest.exe"));

    std::thread getDataThread(&MainServerManager::_get_data_from_request, this, myTestRequest);
    getDataThread.detach();
}

Client MainServerManager::get_client_by_user(Database::UserIdType user)
{
    if(users.contains(user)){
        return clients[users.indexOf(user)];
    } else{
        throw new Exceptions::MainServerException(System::String("No such user in users list!!!"),
                                                  Exceptions::MainServerExceptionId_NoSuchUser);
        return Client();
    }
}

Database::UserIdType MainServerManager::get_user_by_client(Client client)
{
    if(clients.contains(client)){
        return users[clients.indexOf(client)];
    } else{
        throw new Exceptions::MainServerException(System::String("No such client in clients list!!!"),
                                                            Exceptions::MainServerExceptionId_NoSuchClient);
        return Database::UserIdType();
    }
}
