#include "chatsettingsdatabasemanager.h"
#include "databasehelper.h"
using namespace Mafia;
using namespace Database;

ChatSettingsDatabaseManager::ChatSettingsDatabaseManager(DatabaseWorker *databaseWorker) : DatabaseManager(databaseWorker, "chats_db")
{
    try {
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            QString request = "CREATE TABLE " + dbName + "("
                    "ID " + get_sql_type<ChatIdType>() + " PRIMARY KEY" + NOT_NULL + ", "
                    "USERS " + get_sql_type<MafiaList<UserIdType>>() + ", "
                    "CAPABILITIES " + get_sql_type<MafiaList<ChatCapability>>() + ", "
                    "TIMESTAMP " + get_sql_type<QDateTime>() + NOT_NULL
                    ")";
            try {
                dbWorker->run_query(request);
            } catch (Exceptions::Exception* exception) {
                switch (exception->get_id()) {
                default:{
                    exception->show();
                    break;
                }
                }
            }
            break;
        }
        default:{
            exception->show();
            break;
        }
        }
    }
}

ChatIdType ChatSettingsDatabaseManager::create_chat()
{
    QString idRequest = "SELECT COALESCE(MAX(ID), 0) FROM " + dbName;

    try {
        QSqlQuery* idQuery = dbWorker->run_query(idRequest);

        if(idQuery->next()){
            ChatIdType id = query_value_to_variable<ChatIdType>(idQuery->value(0)) + 1;
            QString request = "INSERT INTO " + dbName + " (ID, TIMESTAMP, USERS, CAPABILITIES) VALUES (%1, %2, \'\', \'\')";

            request = request.arg(QString::number(id));
            request = request.arg("\'" + QDateTime::currentDateTimeUtc().toString(SQL_DATETIME_FORMAT) + "\'");

            dbWorker->run_query(request);
            return id;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null query answer"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }
}

void ChatSettingsDatabaseManager::add_user_to_chat(UserIdType user, ChatIdType chat, ChatCapability usersCapability)
{
    QString chatRequest = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
        QSqlQuery* chatQuery = dbWorker->run_query(chatRequest);

        if(chatQuery->next()){
            QSqlRecord record = chatQuery->record();
            MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(chatQuery->value(record.indexOf("USERS")));
            MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(chatQuery->value(record.indexOf("CAPABILITIES")));
            if(users.contains(user)){
                throw new Exceptions::DatabaseWorkingException(System::String("Attemp to add user to chat with this user"),
                                                               Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt);
            }
            capabilities.append(usersCapability);
            users.append(user);

            QString request = "UPDATE " + dbName + "\nSET USERS = %1, CAPABILITIES = %2 WHERE (ID = " + QString::number(chat) + ")";
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(users).toStdString()) + "\'");
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<ChatCapability>(capabilities).toStdString()) + "\'");

            dbWorker->run_query(request);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null sql answer!"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

MafiaList<ChatIdType> ChatSettingsDatabaseManager::get_chats_with(MafiaList<ChatIdType> ids, MafiaList<UserIdType> users,
                                                                  FilterType usersFilter, QDateTime createdAfter, QDateTime createdBefore)
{
    QString request = "SELECT ID FROM " + dbName + " WHERE (TRUE";

    if(ids.length() > 0){
        request += " AND (";
        for(int i = 0; i < ids.length(); i++){
            request += "ID = " + QString::number(ids[i]);

            if(i != ids.length() - 1){
                request += " OR ";
            } else{
                request += ")";
            }
        }
    }

    if(users.length() > 0){
        request += " AND (";
        MafiaList<UserIdType> oneUserList = MafiaList<UserIdType>() << users[0];
        for(int i = 0; i < users.length(); i++){
            oneUserList[0] = users[i];
            request += "CONTAINS(USERS, " + QString::fromStdString(qbytearray_from_qlist<UserIdType>(oneUserList).toStdString());

            if(i != ids.length() - 1){
                switch (usersFilter) {
                case FilterType_OR:{
                    request += " OR ";
                    break;
                }
                case FilterType_AND:{
                    request += " AND ";
                    break;
                }
                case FilterType_NONE:{
                    throw new Exceptions::DatabaseWorkingException(System::String("Incorrect filter!"),
                                                                   Exceptions::DatabaseWorkingExceptionId_UnknownFilterType);
                        break;
                    }
                }

            } else{
                request += ")";
            }
        }
    }

    request += " AND (TIMESTAMP >= " + createdAfter.toString(SQL_DATETIME_FORMAT) + ")";
    request += " AND (TIMESTAMP <= " + createdBefore.toString(SQL_DATETIME_FORMAT) + ")";
    request += ")";

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        MafiaList<ChatIdType> result = MafiaList<ChatIdType>();

        while(query->next()){
            result.append(query_value_to_variable<ChatIdType>(query->value(0)));
        }

        return result;

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<ChatIdType>();
        }
        }
    }

}

void ChatSettingsDatabaseManager::set_capability(UserIdType user, ChatIdType chat, ChatCapability newCapability)
{
    QString chatRequest = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
        QSqlQuery* chatQuery = dbWorker->run_query(chatRequest);

        if(chatQuery->next()){
            QSqlRecord record = chatQuery->record();
            MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(chatQuery->value(record.indexOf("USERS")));
            MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(chatQuery->value(record.indexOf("CAPABILITIES")));
            if(users.contains(user)){
                capabilities[users.indexOf(user)] = newCapability;
            } else{
                throw new Exceptions::DatabaseWorkingException(System::String("Attemp to edit users capability in chat without this user"),
                                                               Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt);
            }


            QString request = "UPDATE " + dbName + "\nSET CAPABILITIES = %1 WHERE (ID = " + QString::number(chat) + ")";
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<ChatCapability>(capabilities).toStdString()) + "\'");

            dbWorker->run_query(request);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null sql answer!"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void ChatSettingsDatabaseManager::remove_user_from_chat(UserIdType user, ChatIdType chat)
{
    QString chatRequest = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
        QSqlQuery* chatQuery = dbWorker->run_query(chatRequest);

        if(chatQuery->next()){
            QSqlRecord record = chatQuery->record();
            MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(chatQuery->value(record.indexOf("USERS")));
            MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(chatQuery->value(record.indexOf("CAPABILITIES")));
            if(users.contains(user)){
                capabilities.removeAt(users.indexOf(user));
                users.removeOne(user);
            } else{
                throw new Exceptions::DatabaseWorkingException(System::String("Attemp to remove user from chat without this user"),
                                                               Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt);
            }


            QString request = "UPDATE " + dbName + "\nSET USERS = %1, CAPABILITIES = %2 WHERE (ID = " + QString::number(chat) + ")";
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(users).toStdString()) + "\'");
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<ChatCapability>(capabilities).toStdString()) + "\'");

            dbWorker->run_query(request);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null sql answer!"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

MafiaList<Chat> ChatSettingsDatabaseManager::get_all_chats()
{
    QString request = "SELECT * FROM " + dbName;
    try {
        QSqlQuery* query = dbWorker->run_query(request);
        return get_request_chat(query);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<Chat>();
        }
        }
    }
}

MafiaList<Chat> ChatSettingsDatabaseManager::get_users_chat(UserIdType user)
{
    MafiaList<UserIdType> oneUser = MafiaList<UserIdType>() << user;
    QString oneUserString = QString::fromStdString(qbytearray_from_qlist<UserIdType>(oneUser).toStdString());

    QString request = "SELECT * FROM " + dbName + " WHERE (CONTAINS(USERS, \'" + oneUserString + "\'))";
    try {
        QSqlQuery* query = dbWorker->run_query(request);
        return get_request_chat(query);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<Chat>();
        }
        }
    }
}

Chat ChatSettingsDatabaseManager::get_chat(ChatIdType chat)
{

    QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";
    try {
        QSqlQuery* query = dbWorker->run_query(request);
        MafiaList<Chat> chats = get_request_chat(query);
        if(chats.length() == 1){
            return chats[0];
        } else{
            if(chats.length() < 1){
                throw new Exceptions::DatabaseWorkingException(System::String("No chat with this ID"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
            } else {
                throw new Exceptions::DatabaseWorkingException(System::String("Something impossible happened! Many chats with same ID"),
                                                               Exceptions::DatabaseWorkingExceptionId_SQlQuery);
            }
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return Chat();
        }
        }
    }
}

bool ChatSettingsDatabaseManager::can_send_message(UserIdType user, ChatIdType chat)
{
    try {
        ChatCapability usersCapability = get_capability(user, chat);
        switch (usersCapability) {
        case ChatCapabilities_Admin:{
            return true;
        }
        case ChatCapabilities_Editor:{
            return true;
        }
        case ChatCapabilities_Speaker:{
            return true;
        }
        case ChatCapabilities_Watcher:{
            return false;
        }
        case ChatCapabilities_None:{
            return false;
        }
        default:{
            throw new Exceptions::DatabaseWorkingException(System::String("Unknown chat capability"),
                                                           Exceptions::DatabaseWorkingExceptionId_UnknownChatCapability);
        }
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

bool ChatSettingsDatabaseManager::can_read_message(UserIdType user, ChatIdType chat)
{
    try {
        ChatCapability usersCapability = get_capability(user, chat);
        switch (usersCapability) {
        case ChatCapabilities_Admin:{
            return true;
        }
        case ChatCapabilities_Editor:{
            return true;
        }
        case ChatCapabilities_Speaker:{
            return true;
        }
        case ChatCapabilities_Watcher:{
            return true;
        }
        case ChatCapabilities_None:{
            return false;
        }
        default:{
            throw new Exceptions::DatabaseWorkingException(System::String("Unknown chat capability"),
                                                           Exceptions::DatabaseWorkingExceptionId_UnknownChatCapability);
        }
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

bool ChatSettingsDatabaseManager::can_edit_users(UserIdType user, ChatIdType chat)
{
    try {
        ChatCapability usersCapability = get_capability(user, chat);
        switch (usersCapability) {
        case ChatCapabilities_Admin:{
            return true;
        }
        case ChatCapabilities_Editor:{
            return true;
        }
        case ChatCapabilities_Speaker:{
            return false;
        }
        case ChatCapabilities_Watcher:{
            return false;
        }
        case ChatCapabilities_None:{
            return false;
        }
        default:{
            throw new Exceptions::DatabaseWorkingException(System::String("Unknown chat capability"),
                                                           Exceptions::DatabaseWorkingExceptionId_UnknownChatCapability);
        }
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

bool ChatSettingsDatabaseManager::can_edit_messages(UserIdType user, ChatIdType chat)
{
    try {
        ChatCapability usersCapability = get_capability(user, chat);
        switch (usersCapability) {
        case ChatCapabilities_Admin:{
            return true;
        }
        case ChatCapabilities_Editor:{
            return true;
        }
        case ChatCapabilities_Speaker:{
            return false;
        }
        case ChatCapabilities_Watcher:{
            return false;
        }
        case ChatCapabilities_None:{
            return false;
        }
        default:{
            throw new Exceptions::DatabaseWorkingException(System::String("Unknown chat capability"),
                                                           Exceptions::DatabaseWorkingExceptionId_UnknownChatCapability);
        }
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

ChatCapability ChatSettingsDatabaseManager::get_capability(UserIdType user, ChatIdType chat)
{
    QString request = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        if(query->next()){
            MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(query->value(query->record().indexOf("USERS")));
            MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(
                                                     query->value(query->record().indexOf("CAPABILITIES")));

            if(users.contains(user)){
                return capabilities[users.indexOf(user)];
            } else{
                return ChatCapabilities_None;
            }
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null query answer"),
                                                           Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return ChatCapabilities_None;
        }
        }
    }
}

MafiaList<Chat> ChatSettingsDatabaseManager::get_request_chat(QSqlQuery *query)
{
    MafiaList<Chat> chats = MafiaList<Chat>();

    QSqlRecord record = query->record();

    while (query->next()) {
        Chat current = Chat();

        current.id = query_value_to_variable<ChatIdType>(query->value(record.indexOf("ID")));
        current.users = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("USERS")));
        current.creationTime = query_value_to_variable<QDateTime>(query->value(record.indexOf("TIMESTAMP")));
        current.usersCapabilities = query_value_to_variable<MafiaList<ChatCapability>>(query->value(record.indexOf("CAPABILITIES")));

        chats.append(current);
    }

    return chats;
}

void Chat::show()
{
    std::cout << "-------------------------\nCHAT" <<
                 "\nid = " << this->id <<
                 "\ntime = " << this->creationTime.toString(SQL_DATETIME_FORMAT).toStdString() <<
                 "\nusers:";
    for(int j = 0; j < this->users.length(); j++){
        std::cout << "\n    id = " << this->users[j] <<
                     "\n    role = " << this->usersCapabilities[j];
    }
    std::cout << std::endl;
}
