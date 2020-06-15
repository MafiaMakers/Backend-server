#include "userdatabaseworker.h"
#include "Exceptions/databaseworkingexception.h"
#include "user.h"
#include "System/SHA_256.h"
#include "Gameplay/game_s.h"
#include <iostream>

using namespace Mafia;
using namespace Database;


const QString UserDatabaseWorker::dbName = "users_db";

UserDatabaseWorker::UserDatabaseWorker(DatabaseWorker* _dbWorker)
{
    dbWorker = _dbWorker;
    try {
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{

            QString query = "CREATE TABLE " + dbName + " ("
                    "ID " + DatabaseWorker::get_sql_type(typeid(User().id).hash_code()) + " PRIMARY KEY" + NOT_NULL + ", "
                    "NICKNAME " + DatabaseWorker::get_sql_type(typeid (User().nickname).hash_code()) + NOT_NULL + ", "
                    "EMAIL " + DatabaseWorker::get_sql_type(typeid (User().email).hash_code()) + UNIQUE + NOT_NULL + ", "
                    "PASSWORD_HASH " + DatabaseWorker::get_sql_type(typeid (User().passwordHash).hash_code()) + NOT_NULL + ", "
                    "PASSWORD_SALT " + DatabaseWorker::get_sql_type(typeid (User().salt).hash_code()) + NOT_NULL + ", "
                    "IS_CONFIRMED " + DatabaseWorker::get_sql_type(typeid (User().isConfirmed).hash_code()) + NOT_NULL + ", "
                    "IS_AUTHORIZED " + DatabaseWorker::get_sql_type(typeid (User().authorized).hash_code()) + NOT_NULL + ", "
                    "ACCOUNT_TYPE " + DatabaseWorker::get_sql_type(typeid (User().accountType).hash_code()) + NOT_NULL + ", "
                    "LOGIN_DATE_TIME " + DatabaseWorker::get_sql_type(typeid (User().loginDateTime).hash_code()) + NOT_NULL + ", "
                    "ACHIEVEMENT " + DatabaseWorker::get_sql_type(typeid (User().achievement).hash_code()) + ", "
                    "CONFIRMATION_KEY " + DatabaseWorker::get_sql_type(typeid (User().confirmationKey).hash_code()) + NOT_NULL + ", "
                    "TRANSACTIONS " + DatabaseWorker::get_sql_type(typeid (User().transactions).hash_code(), MAX_TRANSACTIONS_COUNT) + ", "
                    "ALL_GAMES " + DatabaseWorker::get_sql_type(typeid (User().allGames).hash_code(), MAX_GAMES_COUNT) + NOT_NULL + ", "
                    "VICTORIES_BY_ROLES " + DatabaseWorker::get_sql_type(typeid (User().victoriesByRoles).hash_code(), Gameplay::Role_Last) + ", "
                    "DEFEATES_BY_ROLES " + DatabaseWorker::get_sql_type(typeid (User().defeatesByRoles).hash_code(), MAX_GAMES_COUNT) + ", "
                    "ALL_CHATS " + DatabaseWorker::get_sql_type(typeid (User().chats).hash_code(), MAX_CHATS_COUNT) + ");";
            try {
                dbWorker->run_query(query);
            } catch (Exceptions::Exception * exception) {
                exception->show();
            }

            break;
        }
        default:{
            exception->show();
        }
        }
    }
}

UserIdType UserDatabaseWorker::add_user(QString nickname, QString email, QString password)
{
    User newUser = User();

    newUser.nickname = nickname;
    newUser.email = email;
    newUser.salt = generate_salt();

    QString addedPassword = password + newUser.salt + localParameter;

    std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());

    newUser.passwordHash = QString::fromStdString(hash);

    newUser.isConfirmed = Status_NotConfirmed;
    newUser.authorized = true;
    newUser.allGames = QList<GameIdType>();
    newUser.accountType = AccountType_Free;
    newUser.achievement = Achievement_None;
    newUser.victoriesByRoles = QList<int>();
    newUser.defeatesByRoles = QList<int>();

    for(int i = Gameplay::Role_First; i < Gameplay::Role_Last; i++){
        newUser.victoriesByRoles.append(0);
        newUser.defeatesByRoles.append(0);
    }

    newUser.chats = QList<ChatIdType>();
    newUser.transactions = QList<TransactionIdType>();
    newUser.loginDateTime = QDateTime::currentDateTimeUtc();
    newUser.confirmationKey = generate_confirmation_key();

    try {
        return add_user(newUser);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }

}

bool UserDatabaseWorker::login_user(UserIdType id, QString password)
{
    try {
        User user = get_user(id);

        QString addedPassword = password + user.salt + localParameter;

        std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());

        if(user.passwordHash == QString::fromStdString(hash)){
            QString request = "UPDATE " + dbName + "\nSET IS_AUTHORIZED = TRUE\nWHERE (ID =" + QString::number(id) + " )";
            dbWorker->run_query(request);
            return true;
        } else{
            return false;
        }


    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return false;
        }
        }
    }
}

void UserDatabaseWorker::logout_user(UserIdType id)
{
    QString request = "UPDATE " + dbName + "\nSET IS_AUTHORIZED = FALSE\nWHERE (ID =" + QString::number(id) + " )";
    try {
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }

}

bool UserDatabaseWorker::confirm_user(UserIdType id, QString confirmationKey)
{
    try {
        QString request = "SELECT CONFIRMATION_KEY FROM \'" + dbName + "\' WHERE (ID = " + QString::number(id) + ")";
        QSqlQuery* query = dbWorker->run_query(request);
        if(query->next()){
            QString realKey = query->value(0).toString();

            return realKey == confirmationKey;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return false;
        }
        }
    }
}

User UserDatabaseWorker::get_user(UserIdType id)
{
    try {
        QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

        QSqlQuery* query = dbWorker->run_query(request);

        QSqlRecord record = query->record();
        if(query->next()){
            User currentUser = User();

            currentUser.id = id;
            currentUser.salt = query->value(record.indexOf("PASSWORD_SALT")).toString();

            currentUser.chats = qlist_from_byte_array<ChatIdType>(query->value(record.indexOf("ALL_CHATS")).toByteArray());

            currentUser.email = query->value(record.indexOf("EMAIL")).toString();
            currentUser.allGames = qlist_from_byte_array<GameIdType>(query->value(record.indexOf("ALL_GAMES")).toByteArray());
            currentUser.nickname = query->value(record.indexOf("NICKNAME")).toString();

            currentUser.authorized = query->value(record.indexOf("IS_AUTHORIZED")).toBool();

            currentUser.accountType = (AccountType)query->value(record.indexOf("ACCOUNT_TYPE")).toInt();
            currentUser.achievement = (Achievement)query->value(record.indexOf("ACHIEVEMENT")).toInt();
            currentUser.isConfirmed = (Status)query->value(record.indexOf("IS_CONFIRMED")).toInt();
            currentUser.passwordHash = query->value(record.indexOf("PASSWORD_HASH")).toString();

            currentUser.transactions = qlist_from_byte_array<TransactionIdType>(query->value(record.indexOf("TRANSACTIONS")).toByteArray());
            currentUser.loginDateTime = query->value(record.indexOf("LOGIN_DATE_TIME")).toDateTime();

            currentUser.confirmationKey = query->value(record.indexOf("CONFIRMATION_KEY")).toString();
            currentUser.defeatesByRoles = qlist_from_byte_array<int>(query->value(record.indexOf("DEFEATES_BY_ROLES")).toByteArray());
            currentUser.victoriesByRoles = qlist_from_byte_array<int>(query->value(record.indexOf("VICTORIES_BY_ROLES")).toByteArray());

            return currentUser;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return User();
        }
        }
    }
}

void UserDatabaseWorker::add_transaction(Transaction transaction)
{
    try {
        QString request = "SELECT TRANSACTIONS FROM " + dbName + " WHERE (ID = " + QString::number(transaction.buyer) + ")";

        QSqlQuery* query = dbWorker->run_query(request);

        if(query->next()){
            QList<TransactionIdType> allTransactions = qlist_from_byte_array<TransactionIdType>(query->value(0).toByteArray());
            allTransactions.append(transaction.id);

            QByteArray transactionsData = qbytearray_from_qlist<TransactionIdType>(allTransactions);

            QString passRequest = "UPDATE " + dbName +
                    "\nSET TRANSACTIONS = \'" + QString::fromStdString(transactionsData.toStdString()) +
                    "\'\nWHERE (ID = " + QString::number(transaction.id) + ")";

            dbWorker->run_query(passRequest);

        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }

}

void UserDatabaseWorker::change_password(UserIdType id, QString newPassword)
{
    try {
        QString authorizedRequest = "SELECT IS_AUTHORIZED FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
        QSqlQuery* authoizedQuery = dbWorker->run_query(authorizedRequest);
        if(authoizedQuery->next()){
            if(authoizedQuery->value(0).toBool()){
                QString saltRequest = "SELECT PASSWORD_SALT FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
                QSqlQuery* saltQuery = dbWorker->run_query(saltRequest);

                if(saltQuery->next()){
                    QString salt = saltQuery->value(0).toString();

                    QString addedPassword = newPassword + salt + localParameter;

                    std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());

                    QString passwordHash = QString::fromStdString(hash);

                    QString request = "UPDATE " + dbName + "\nSET PASSWORD_HASH = \'" + passwordHash + "\'\nWHERE (ID = " + QString::number(id) + ")";
                    dbWorker->run_query(request);

                } else{
                    throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
                }
            } else{
                throw new Exceptions::DatabaseWorkingException(System::String("User must be authorized to change password"), Exceptions::DatabaseWorkingExceptionId_NotAuthorizedAction);
            }

        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }

    } catch (Exceptions::Exception* exception) {
        switch(exception->get_id()){
        default:{
            throw exception;
            return;
        }
        }
    }
}

UserIdType UserDatabaseWorker::get_id(QString email)
{
    QString selectRequest = "SELECT ID FROM " + UserDatabaseWorker::dbName + " WHERE (EMAIL = \'" + email + "\')";

    try {
        QSqlQuery* query = dbWorker->run_query(selectRequest);

        if(query->next()){
            UserIdType id = query->value(0).toInt();
            return id;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
            return -1;
        }



    } catch (Exceptions::Exception * exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }


}

void UserDatabaseWorker::add_achievement(UserIdType id, Achievement achievement)
{
    QString request = "UPDATE " + dbName + "\nSET ACHIEVEMENT = " + QString::number(achievement) + "\nWHERE (ID = " + QString::number(id) + ")";

    try {
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return;
        }
        }
    }

}

void UserDatabaseWorker::add_user_to_chat(UserIdType userId, ChatIdType chatId)
{
    QString chatsRequest = "SELECT ALL_CHATS FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
        QSqlQuery* chatsQuery = dbWorker->run_query(chatsRequest);

        if(chatsQuery->next()){
            QList<ChatIdType> chats = qlist_from_byte_array<ChatIdType>(chatsQuery->value(0).toByteArray());

            chats.append(chatId);

            QByteArray newChats = qbytearray_from_qlist<ChatIdType>(chats);
            QString request = "UPDATE " + dbName + "\nSET ALL_CHATS = \'" + QString::fromStdString(newChats.toStdString()) + "\'\n WHERE (ID = " + QString::number(userId) + ")";
            dbWorker->run_query(request);
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }
    } catch (Exceptions::Exception * exception) {
        switch(exception->get_id()){
        default:{
            throw exception;
            return;
        }
        }
    }

}

void UserDatabaseWorker::set_account_type(UserIdType id, AccountType newAccountType)
{
    try {
        QString request = "UPDATE " + dbName + "\nSET ACCOUNT_TYPE = " + QString::number(newAccountType) + "\nWHERE (ID = " + QString::number(id) + ")";
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return;
        }
        }
    }
}

bool UserDatabaseWorker::change_email(UserIdType id, QString newEmail)
{
    QString getRequest = "SELECT EMAIL FROM " + dbName;

    try {
        QSqlQuery* getQuery = dbWorker->run_query(getRequest);

        while(getQuery->next()){
            if(getQuery->value(0).toString() == newEmail){
                return false;
            }
        }

        QString setRequest = "UPDATE " + dbName + "\nSET EMAIL = \'" + newEmail + "\'\nWHERE (ID = " + QString::number(id) + ")";

        dbWorker->run_query(setRequest);

        return true;

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return false;
        }
        }
    }

}

void UserDatabaseWorker::change_nickname(UserIdType id, QString newNickname)
{
    QString request = "UPDATE " + dbName + "\nSET NICKNAME = \'" + newNickname + "\'\nWHERE (ID = " + QString::number(id) + ")";

    try {
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

UserIdType UserDatabaseWorker::add_user(User user)
{
    QString request = "INSERT INTO " + dbName + "(ID, NICKNAME, EMAIL, PASSWORD_HASH, PASSWORD_SALT, IS_CONFIRMED, IS_AUTHORIZED, "
                                                "ACCOUNT_TYPE, LOGIN_DATE_TIME, ACHIEVEMENT, CONFIRMATION_KEY, TRANSACTIONS, "
                                                "ALL_GAMES, VICTORIES_BY_ROLES, DEFEATES_BY_ROLES, ALL_CHATS)"
                                                "VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9, %11, %12, %13, %14, %15, %16, %17)";

    QString userIdRequest = "SELECT MAX(ID) FROM " + dbName;
    try {
        QSqlQuery* userIdQuery = dbWorker->run_query(userIdRequest);

        if(userIdQuery->next()){
            user.id = userIdQuery->value(0).toInt() + 1;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }

    request = request.arg(QString::number(user.id));
    request = request.arg("\'" + user.nickname + "\'");
    request = request.arg("\'" + user.email + "\'");
    request = request.arg("\'" + user.passwordHash + "\'");
    request = request.arg("\'" + user.salt + "\'");
    request = request.arg(QString::number(user.isConfirmed));
    request = request.arg(user.authorized ? "TRUE" : "FALSE");
    request = request.arg(QString::number(user.accountType));
    request = request.arg("\'" + user.loginDateTime.toString("yyyy-MM-dd hh:mm:ss") + "\'");
    //std::cout << user.loginDateTime.toString("dd-mm").toStdString() << " - this is login datetime\n";
    request = request.arg(QString::number(user.achievement));
    request = request.arg("\'" + user.confirmationKey + "\'");

    QString transactions = QString::fromStdString(qbytearray_from_qlist<TransactionIdType>(user.transactions).toStdString());
    request = request.arg("\'" + transactions + "\'");

    QString allGames = QString::fromStdString(qbytearray_from_qlist<GameIdType>(user.allGames).toStdString());
    request = request.arg("\'" + allGames + "\'");

    QString victories = QString::fromStdString(qbytearray_from_qlist<int>(user.victoriesByRoles).toStdString());
    request = request.arg("\'" + victories + "\'");

    QString defeates = QString::fromStdString(qbytearray_from_qlist<int>(user.defeatesByRoles).toStdString());
    request = request.arg("\'" + defeates + "\'");

    QString allChats = QString::fromStdString(qbytearray_from_qlist<ChatIdType>(user.chats).toStdString());
    request = request.arg("\'" + allChats + "\'");

    try {
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }

    return user.id;
}

QString UserDatabaseWorker::generate_salt()
{
    const int saltSize = 10;

    QString salt = "";

    for(int i = 0; i < saltSize; i++){
        salt += (char)((char)'A' + (char)(qrand() % 26));
    }

    return salt;
}

QString UserDatabaseWorker::generate_confirmation_key()
{
    const int confirmationKeySize = 20;

    QString confirmationKey = "";

    for(int i = 0; i < confirmationKeySize; i++){
        confirmationKey += (char)((char)'A' + (char)(qrand() % 26));
    }

    return confirmationKey;
}

template<class T>
QList<T> UserDatabaseWorker::qlist_from_byte_array(QByteArray data)
{
    char* dataCharPtr = data.data();

    char current = 0;

    char* trueData = new char[data.size() / 2];

    for(int i = 0; i < data.size(); i++){
        if(i % 2 == 0){
            current += 16 * (dataCharPtr[i] - 'M');
        } else{
            current += (dataCharPtr[i] - 'M');
            trueData[i / 2] = current;
            current = 0;
        }
    }

    T*dataPtr = (T*)trueData;
    if(data.size() % (2 * sizeof(T)) == 0){
        int size = data.size() / sizeof(T) / 2;
        QList<T> res = QList<T>();
        for(int i = 0; i < size; i++){
            res.append(dataPtr[i]);
        }

        return res;
    } else{
        throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        return QList<T>();
    }
}

template<class T>
QByteArray UserDatabaseWorker::qbytearray_from_qlist(QList<T> data)
{
    char* dataPtr = new char[data.length() * sizeof (T) * 2];

    for(int i = 0; i < data.length(); i++){
        for(unsigned int j = 0; j < sizeof(T); j++){
            dataPtr[i * sizeof(T) * 2 + j * 2] = 'M' + (((char*)&(data[i]))[j] / 16);
            dataPtr[i * sizeof(T) * 2 + j * 2 + 1] = 'M' + (((char*)&(data[i]))[j] % 16);
        }
    }

    return QByteArray(dataPtr, data.length() * sizeof (T) * 2);
}


void UserDatabaseWorker::register_game(Gameplay::Game game)
{
    try {
        for(int i = 0; i < game.users.length(); i++){
            Gameplay::GamePersonalResult res = Gameplay::result_by_role_and_res(game.roles[i], game.result);
            register_game(game.users[i], game.id, game.roles[i], res);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }

}


void UserDatabaseWorker::register_game(UserIdType userId, GameIdType gameId, Gameplay::Role role, Gameplay::GamePersonalResult result)
{
    QString allGamesRequest = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
        QSqlQuery* allGamesQuery = dbWorker->run_query(allGamesRequest);
        QSqlRecord allGamesRecord = allGamesQuery->record();
        if(allGamesQuery->next()){
            QList<GameIdType> games = qlist_from_byte_array<GameIdType>(allGamesQuery->value(allGamesRecord.indexOf("ALL_GAMES")).toByteArray());
            games.append(gameId);

            QString request = "UPDATE " + dbName +
                    "\nSET ALL_GAMES = \'" + QString::fromStdString(qbytearray_from_qlist<GameIdType>(games).toStdString()) +
                    "\'\nWHERE (ID = " + QString::number(userId) + ")";

            dbWorker->run_query(request);



            switch (result) {
            case Gameplay::GamePersonalResult_Defeat:{
                QList<int> defs = qlist_from_byte_array<int>(allGamesQuery->value(allGamesRecord.indexOf("DEFEATES_BY_ROLES")).toByteArray());
                defs[role]++;

                QString defsRequest = "UPDATE " + dbName +
                        "\nSET DEFEATES_BY_ROLES = \'" + QString::fromStdString(qbytearray_from_qlist<int>(defs).toStdString()) +
                        "\'\nWHERE (ID = " + QString::number(userId) + ")";

                dbWorker->run_query(defsRequest);
                break;
            }
            case Gameplay::GamePersonalResult_Victory:{
                QList<int> victs = qlist_from_byte_array<int>(allGamesQuery->value(allGamesRecord.indexOf("VICTORIES_BY_ROLES")).toByteArray());
                victs[role]++;

                QString victRequest = "UPDATE " + dbName +
                        "\nSET VICTORIES_BY_ROLES = \'" + QString::fromStdString(qbytearray_from_qlist<int>(victs).toStdString()) +
                        "\'\nWHERE (ID = " + QString::number(userId) + ")";

                dbWorker->run_query(victRequest);
                break;
            }
            default:{}
            }

        } else{

        }

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return;
        }
        }
    }

}
