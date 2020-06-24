#include "chatdatabasemanager.h"
#include "Exceptions/databaseworkingexception.h"
#include "databasehelper.h"

using namespace Mafia;
using namespace Database;

ChatDatabaseManager::ChatDatabaseManager(DatabaseWorker *databaseWorker, QObject *parent)
    : QObject(parent), DatabaseManager(databaseWorker, "messages_db")
{
    try {
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            QString request = "CREATE TABLE " + dbName + "("
                    "ID " + get_sql_type<MessageIdType>() + " PRIMARY KEY" + NOT_NULL + ", "
                    "SENDER " + get_sql_type<UserIdType>() + NOT_NULL + ", "
                    "TO_CHAT " + get_sql_type<ChatIdType>() + NOT_NULL + ", "
                    "DATA " + get_sql_type<QString>() + NOT_NULL + ", "
                    "TIMESTAMP " + get_sql_type<QDateTime>() + NOT_NULL + ", "
                    "FEATURE " + get_sql_type<ChatFeature>() + NOT_NULL + ", "
                    "ANSWER_FOR " + get_sql_type<MafiaList<MessageIdType>>(MAX_ANSWER_MESSAGE_COUNT) + ", "
                    "READ_USERS " + get_sql_type<MafiaList<UserIdType>>(MAX_USERS_IN_CHAT) + ")";
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

MafiaList<ChatMessage> ChatDatabaseManager::get_last_messages(ChatIdType chatId, int messagesCount)
{
    QString request = "SELECT * FROM " + dbName + " WHERE (TO_CHAT = " + QString::number(chatId) + ") ORDER BY ID DESC";

    try {
        QSqlQuery* query = dbWorker->run_query(request);
        QSqlRecord record = query->record();
        MafiaList<ChatMessage> messages = MafiaList<ChatMessage>();

        while(query->next() && (messages.length() < messagesCount)){
            ChatMessage message = ChatMessage();

            message.id = query_value_to_variable<MessageIdType>(query->value(record.indexOf("ID")));
            message.from = query_value_to_variable<UserIdType>(query->value(record.indexOf("SENDER")));
            message.toChat = query_value_to_variable<ChatIdType>(query->value(record.indexOf("TO_CHAT")));
            message.feature = query_value_to_variable<ChatFeature>(query->value(record.indexOf("FEATURE")));
            message.timestamp = query_value_to_variable<QDateTime>(query->value(record.indexOf("TIMESTAMP")));
            message.answerFor = query_value_to_variable<MafiaList<MessageIdType>>(query->value(record.indexOf("ANSWER_FOR")));
            message.readUsers = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("READ_USERS")));
            message.data = query_value_to_variable<QString>(query->value(record.indexOf("DATA")));
            messages.append(message);
        }

        return messages;

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<ChatMessage>();
        }
        }
    }
}

MessageIdType ChatDatabaseManager::add_message(ChatMessage message)
{
    QString request = "INSERT INTO " + dbName + " (ID, SENDER, TO_CHAT, TIMESTAMP, FEATURE, READ_USERS, ANSWER_FOR, DATA) VALUES (%1, %2, %3, %4, %5, %6, %7, %8)";

    QString idRequest = "SELECT COALESCE(MAX(ID), 0) FROM " + dbName;
    try {
        QSqlQuery* idQuery = dbWorker->run_query(idRequest);

        if(idQuery->next()){
            MessageIdType id = query_value_to_variable<MessageIdType>(idQuery->value(0)) + 1;
            message.id = id;

            message.readUsers = MafiaList<UserIdType>();
            message.timestamp = QDateTime::currentDateTimeUtc();

            request = request.arg(QString::number(message.id));
            request = request.arg(QString::number(message.from));
            request = request.arg(QString::number(message.toChat));
            request = request.arg("\'" + message.timestamp.toString(SQL_DATETIME_FORMAT) + "\'");
            request = request.arg(QString::number(message.feature));
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(message.readUsers).toStdString()) + "\'");
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<MessageIdType>(message.answerFor).toStdString()) + "\'");
            request = request.arg("\'" + message.data + "\'");

            dbWorker->run_query(request);

            emit on_message_sent(message);
            return message.id;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null sql answer"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
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

void ChatDatabaseManager::delete_message(MessageIdType id)
{
    try {
        QString infoRequest = "SELECT TO_CHAT FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
        QSqlQuery* infoQuery = dbWorker->run_query(infoRequest);

        if(infoQuery->next()){
            ChatIdType chatId = query_value_to_variable<ChatIdType>(infoQuery->value(0));
            QString request = "DELETE FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
            dbWorker->run_query(request);

            emit on_message_deleted(id, chatId);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null request answer for this id"),
                                                           Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void ChatDatabaseManager::edit_message(ChatMessage message)
{
    QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(message.id) + ")";

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        ChatMessage oldVersion = ChatMessage();

        QSqlRecord record = query->record();

        if(query->next()){
            oldVersion.id = query_value_to_variable<MessageIdType>(query->value(record.indexOf("ID")));
            oldVersion.from = query_value_to_variable<UserIdType>(query->value(record.indexOf("SENDER")));
            oldVersion.toChat = query_value_to_variable<ChatIdType>(query->value(record.indexOf("TO_CHAT")));
            oldVersion.feature = query_value_to_variable<ChatFeature>(query->value(record.indexOf("FEATURE")));
            oldVersion.answerFor = query_value_to_variable<MafiaList<MessageIdType>>(query->value(record.indexOf("ANSWER_FOR")));
            oldVersion.readUsers = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("READ_USERS")));
            oldVersion.timestamp = query_value_to_variable<QDateTime>(query->value(record.indexOf("TIMESTAMP")));

            if(message.id != oldVersion.id){
                throw new Exceptions::DatabaseWorkingException(System::String("Some impossible thing happened!!!"),
                                                               Exceptions::DatabaseWorkingExceptionId_MessageParameterMismatch);
            }

            if(message.from != oldVersion.from){
                throw new Exceptions::DatabaseWorkingException(System::String("Sender in edited message is different!!"),
                                                               Exceptions::DatabaseWorkingExceptionId_MessageParameterMismatch);
            }

            if(message.toChat != oldVersion.toChat){
                throw new Exceptions::DatabaseWorkingException(System::String("Destination chat differs in edited and old messages!"),
                                                               Exceptions::DatabaseWorkingExceptionId_MessageParameterMismatch);
            }

            QString updateRequest = "UPDATE " + dbName + "\nSET "
                    "TIMESTAMP = %1, "
                    "FEATURE = %2, "
                    "ANSWER_FOR = %3, "
                    "READ_USERS = \'\', "
                    "DATA = %4"
                    "\nWHERE (ID = " + QString::number(message.id) + ")"
                    ;

            message.timestamp = QDateTime::currentDateTimeUtc();
            message.readUsers = MafiaList<UserIdType>();

            updateRequest = updateRequest.arg("\'" + message.timestamp.toString(SQL_DATETIME_FORMAT) + "\'");
            updateRequest = updateRequest.arg(QString::number(message.feature));
            updateRequest = updateRequest.arg("\'" + QString::fromStdString(qbytearray_from_qlist<MessageIdType>(message.answerFor).toStdString()) + "\'");
            updateRequest = updateRequest.arg("\'" + message.data + "\'");

            dbWorker->run_query(updateRequest);

            emit on_message_edited(message);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Not message with such ID"),
                                                           Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }



    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

void ChatDatabaseManager::message_read(MessageType id, UserIdType readUser)
{
    try {
        QString infoRequest = "SELECT READ_USERS, TO_CHAT FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
        QSqlQuery* infoQuery = dbWorker->run_query(infoRequest);

        if(infoQuery->next()){
            QSqlRecord record = infoQuery->record();
            MafiaList<UserIdType> readUsers = query_value_to_variable<MafiaList<UserIdType>>(infoQuery->value(record.indexOf("READ_USERS")));
            ChatIdType chat = query_value_to_variable<ChatIdType>(infoQuery->value(record.indexOf("TO_CHAT")));
            readUsers.append(readUser);
            QString updateRequest = "UPDATE " + dbName + "\nSET READ_USERS = \'" +
                    QString::fromStdString(qbytearray_from_qlist<UserIdType>(readUsers).toStdString()) + "\'\nWHERE (ID = " +
                    QString::number(id) + ")";

            dbWorker->run_query(updateRequest);

            emit on_message_read(id, readUser, chat);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("No messages of such ID"),
                                                           Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

MafiaList<ChatMessage> ChatDatabaseManager::get_messages(MafiaList<ChatIdType> fromChats,
                                                         MafiaList<UserIdType> possibleSenders,
                                                         MafiaList<ChatFeature> features,
                                                         QString containsData,
                                                         QDateTime sentAfter,
                                                         QDateTime sentBefore)
{
    QString request = "SELECT * FROM " + dbName + " WHERE (TRUE";

    if(fromChats.length() != 0){
        request += " AND (";
        for(int i = 0; i < fromChats.length(); i++){
            request += "TO_CHAT = " + QString::number(fromChats[i]);

            if(i != fromChats.length() - 1){
                request += " OR ";
            } else{
                request += ")";
            }
        }
    }

    if(possibleSenders.length() != 0){
        request += " AND (";
        for(int i = 0; i < possibleSenders.length(); i++){
            request += "SENDER = " + QString::number(possibleSenders[i]);

            if(i != possibleSenders.length() - 1){
                request += " OR ";
            } else{
                request += ")";
            }
        }
    }

    if(features.length() != 0){
        request += " AND (";
        for(int i = 0; i < features.length(); i++){
            request += "FEATURE = " + QString::number(features[i]);

            if(i != features.length() - 1){
                request += " OR ";
            } else{
                request += ")";
            }
        }
    }

    if(containsData != ""){
        request += " AND (CONTAINS(DATA, \'" + containsData + "\')";
    }

    request += " AND (TIMESTAMP >= \'" + sentAfter.toString(SQL_DATETIME_FORMAT) + "\')";
    request += " AND (TIMESTAMP <= \'" + sentBefore.toString(SQL_DATETIME_FORMAT) + "\')";
    request += ")";
    request += " ORDER BY TIMESTAMP DESC";

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        return get_query_messages(query);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

MafiaList<ChatMessage> ChatDatabaseManager::get_query_messages(QSqlQuery *query)
{
    MafiaList<ChatMessage> messages = MafiaList<ChatMessage>();

    QSqlRecord record = query->record();
    while (query->next()) {
        ChatMessage mes = ChatMessage();

        mes.id = query_value_to_variable<MessageIdType>(query->value(record.indexOf("ID")));
        mes.data = query_value_to_variable<QString>(query->value(record.indexOf("DATA")));
        mes.from = query_value_to_variable<UserIdType>(query->value(record.indexOf("SENDER")));
        mes.toChat = query_value_to_variable<ChatIdType>(query->value(record.indexOf("TO_CHAT")));
        mes.feature = query_value_to_variable<ChatFeature>(query->value(record.indexOf("FEATURE")));
        mes.answerFor = query_value_to_variable<MafiaList<MessageIdType>>(query->value(record.indexOf("ANSWER_FOR")));
        mes.readUsers = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("READ_USERS")));
        mes.timestamp = query_value_to_variable<QDateTime>(query->value(record.indexOf("TIMESTAMP")));

        messages.append(mes);
    }

    return messages;
}
