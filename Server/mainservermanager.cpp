#include "mainservermanager.h"
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
        _database_get_all();
    } catch (Exceptions::Exception* exception) {
        exception->show();
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
                                                      (char*)"Some ask",
                                                      9,
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
