#include "userdatabasemanager.h"
#include "Exceptions/databaseworkingexception.h"
#include "user.h"
#include "System/SHA_256.h"
#include "Gameplay/game_s.h"
#include <iostream>

using namespace Mafia;
using namespace Database;


//const QString UserDatabaseWorker::dbName = "users_db";

UserDatabaseManager::UserDatabaseManager(DatabaseWorker* _dbWorker) : DatabaseManager(_dbWorker, "users_db")
{
    try {
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            try {
                QString query = "CREATE TABLE " + dbName + " ("
                        "ID " + get_sql_type<UserIdType>() + " PRIMARY KEY" + NOT_NULL + ", "
                        "NICKNAME " + get_sql_type<QString>() + NOT_NULL + ", "
                        "EMAIL " + get_sql_type<QString>() + UNIQUE + NOT_NULL + ", "
                        "PASSWORD_HASH " + get_sql_type<QString>() + NOT_NULL + ", "
                        "PASSWORD_SALT " + get_sql_type<QString>() + NOT_NULL + ", "
                        "IS_CONFIRMED " + get_sql_type<Status>() + NOT_NULL + ", "
                        "IS_AUTHORIZED " + get_sql_type<AuthorizedStatus>() + NOT_NULL + ", "
                        "ACCOUNT_TYPE " + get_sql_type<AccountType>() + NOT_NULL + ", "
                        "LOGIN_DATE_TIME " + get_sql_type<QDateTime>() + NOT_NULL + ", "
                        "ACHIEVEMENT " + get_sql_type<Achievement>() + ", "
                        "CONFIRMATION_KEY " + get_sql_type<QString>() + NOT_NULL + ", "
                        "TRANSACTIONS " + get_sql_type<MafiaList<TransactionIdType>>(MAX_TRANSACTIONS_COUNT) + ", "
                        "ALL_GAMES " + get_sql_type<MafiaList<GameIdType>>(MAX_GAMES_COUNT) + NOT_NULL + ", "
                        "VICTORIES_BY_ROLES " + get_sql_type<MafiaList<int>>(Gameplay::Role_Last) + ", "
                        "DEFEATES_BY_ROLES " + get_sql_type<MafiaList<int>>(Gameplay::Role_Last) + ", "
                        "ALL_CHATS " + get_sql_type<MafiaList<ChatIdType>>(MAX_CHATS_COUNT) + ");";
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

UserIdType UserDatabaseManager::add_user(QString nickname, QString email, QString password)
{
    User newUser = User();

    newUser.nickname = nickname;
    newUser.email = email;
    newUser.salt = generate_salt();

    QString addedPassword = password + newUser.salt + localParameter;

    std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());

    newUser.passwordHash = QString::fromStdString(hash);

    newUser.isConfirmed = Status_NotConfirmed;
    newUser.authorized = AuthorizedStatus_Authorized;
    newUser.allGames = MafiaList<GameIdType>();
    newUser.accountType = AccountType_Free;
    newUser.achievement = Achievement_None;
    newUser.victoriesByRoles = MafiaList<int>();
    newUser.defeatesByRoles = MafiaList<int>();

    for(int i = Gameplay::Role_First; i < Gameplay::Role_Last; i++){
        newUser.victoriesByRoles.append(0);
        newUser.defeatesByRoles.append(0);
    }

    newUser.chats = MafiaList<ChatIdType>();
    newUser.transactions = MafiaList<TransactionIdType>();
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

bool UserDatabaseManager::login_user(UserIdType id, QString password)
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

void UserDatabaseManager::logout_user(UserIdType id)
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

bool UserDatabaseManager::confirm_user(UserIdType id, QString confirmationKey)
{
    try {
        QString request = "SELECT CONFIRMATION_KEY FROM \'" + dbName + "\' WHERE (ID = " + QString::number(id) + ")";
        QSqlQuery* query = dbWorker->run_query(request);
        if(query->next()){
            QString realKey = query_value_to_variable<QString>(query->value(0));

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

User UserDatabaseManager::get_user(UserIdType id)
{
    try {
        QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

        QSqlQuery* query = dbWorker->run_query(request);

        QSqlRecord record = query->record();

        if(query->next()){
            User currentUser = User();

            currentUser.id = id;
            currentUser.salt = query_value_to_variable<QString>(query->value(record.indexOf("PASSWORD_SALT")));

            currentUser.chats = query_value_to_variable<MafiaList<ChatIdType>>(query->value(record.indexOf("ALL_CHATS")));

            currentUser.email = query_value_to_variable<QString>(query->value(record.indexOf("EMAIL")));
            currentUser.allGames = query_value_to_variable<MafiaList<GameIdType>>(query->value(record.indexOf("ALL_GAMES")));
            currentUser.nickname = query_value_to_variable<QString>(query->value(record.indexOf("NICKNAME")));

            currentUser.authorized = query_value_to_variable<AuthorizedStatus>(query->value(record.indexOf("IS_AUTHORIZED")));

            currentUser.accountType = query_value_to_variable<AccountType>(query->value(record.indexOf("ACCOUNT_TYPE")));
            currentUser.achievement = query_value_to_variable<Achievement>(query->value(record.indexOf("ACHIEVEMENT")));
            currentUser.isConfirmed = query_value_to_variable<Status>(query->value(record.indexOf("IS_CONFIRMED")));
            currentUser.passwordHash = query_value_to_variable<QString>(query->value(record.indexOf("PASSWORD_HASH")));

            currentUser.transactions = query_value_to_variable<MafiaList<TransactionIdType>>(query->value(record.indexOf("TRANSACTIONS")));
            currentUser.loginDateTime = query_value_to_variable<QDateTime>(query->value(record.indexOf("LOGIN_DATE_TIME")));

            currentUser.confirmationKey = query_value_to_variable<QString>(query->value(record.indexOf("CONFIRMATION_KEY")));
            currentUser.defeatesByRoles = query_value_to_variable<MafiaList<int>>(query->value(record.indexOf("DEFEATES_BY_ROLES")));
            currentUser.victoriesByRoles = query_value_to_variable<MafiaList<int>>(query->value(record.indexOf("VICTORIES_BY_ROLES")));

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

void UserDatabaseManager::add_transaction(UserIdType user, TransactionIdType transaction)
{
    try {
        QString request = "SELECT TRANSACTIONS FROM " + dbName + " WHERE (ID = " + QString::number(user) + ")";

        QSqlQuery* query = dbWorker->run_query(request);

        if(query->next()){
            MafiaList<TransactionIdType> allTransactions = query_value_to_variable<MafiaList<TransactionIdType>>(query->value(0));
            allTransactions.append(transaction);

            QByteArray transactionsData = qbytearray_from_qlist<TransactionIdType>(allTransactions);

            QString passRequest = "UPDATE " + dbName +
                    "\nSET TRANSACTIONS = \'" + QString::fromStdString(transactionsData.toStdString()) +
                    "\'\nWHERE (ID = " + QString::number(user) + ")";

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

void UserDatabaseManager::change_password(UserIdType id, QString newPassword)
{
    try {
        QString authorizedRequest = "SELECT IS_AUTHORIZED FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
        QSqlQuery* authoizedQuery = dbWorker->run_query(authorizedRequest);
        if(authoizedQuery->next()){
            if(query_value_to_variable<bool>(authoizedQuery->value(0))){
                QString saltRequest = "SELECT PASSWORD_SALT FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
                QSqlQuery* saltQuery = dbWorker->run_query(saltRequest);

                if(saltQuery->next()){
                    QString salt = query_value_to_variable<QString>(saltQuery->value(0));

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

UserIdType UserDatabaseManager::get_id(QString email)
{
    QString selectRequest = "SELECT ID FROM " + UserDatabaseManager::dbName + " WHERE (EMAIL = \'" + email + "\')";

    try {
        QSqlQuery* query = dbWorker->run_query(selectRequest);

        if(query->next()){
            UserIdType id = query_value_to_variable<UserIdType>(query->value(0));
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

void UserDatabaseManager::add_achievement(UserIdType id, Achievement achievement)
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

void UserDatabaseManager::add_user_to_chat(UserIdType userId, ChatIdType chatId)
{
    QString chatsRequest = "SELECT ALL_CHATS FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
        QSqlQuery* chatsQuery = dbWorker->run_query(chatsRequest);

        if(chatsQuery->next()){
            MafiaList<ChatIdType> chats = query_value_to_variable<MafiaList<ChatIdType>>(chatsQuery->value(0));

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

void UserDatabaseManager::remove_user_from_chat(UserIdType userId, ChatIdType chatId)
{
    QString chatsRequest = "SELECT ALL_CHATS FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
        QSqlQuery* chatsQuery = dbWorker->run_query(chatsRequest);

        if(chatsQuery->next()){
            MafiaList<ChatIdType> chats = query_value_to_variable<MafiaList<ChatIdType>>(chatsQuery->value(0));

            if(chats.contains(chatId)){
                chats.removeOne(chatId);
                QByteArray newChats = qbytearray_from_qlist<ChatIdType>(chats);
                QString request = "UPDATE " + dbName + "\nSET ALL_CHATS = \'" + QString::fromStdString(newChats.toStdString()) + "\'\n WHERE (ID = " + QString::number(userId) + ")";
                dbWorker->run_query(request);
            } else{
                throw new Exceptions::DatabaseWorkingException(System::String("This user is not in this chat"),
                                                               Exceptions::DatabaseWorkingExceptionId_DeleteMissedItemAttempt);
            }

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

void UserDatabaseManager::set_account_type(UserIdType id, AccountType newAccountType)
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

bool UserDatabaseManager::change_email(UserIdType id, QString newEmail)
{
    QString getRequest = "SELECT EMAIL FROM " + dbName;

    try {
        QSqlQuery* getQuery = dbWorker->run_query(getRequest);

        while(getQuery->next()){
            if(query_value_to_variable<QString>(getQuery->value(0)) == newEmail){
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

void UserDatabaseManager::change_nickname(UserIdType id, QString newNickname)
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

MafiaList<User> UserDatabaseManager::get_users(MafiaList<UserIdType> ids,
                                               Status userStatus,
                                               Achievement userAchievement,
                                               AuthorizedStatus authorizedNow,
                                               QString nickname,
                                               QDateTime loginAfter,
                                               QDateTime loginBefore)
{
    MafiaList<User> users = MafiaList<User>();

    QString request = "SELECT * FROM " + dbName + " WHERE (TRUE";

    if(ids.length() != 0){
        request += " AND (";
        for(int i = 0; i < ids.length(); i++){
            request += "(ID = " + QString::number(ids[i]) + ")";
            if(i != ids.length() - 1){
                request += " OR ";
            } else{
                request += ")";
            }
        }
    }

    if(userStatus != Status_Any){
        request += " AND (IS_CONFIRMED = " + QString::number(userStatus) + ")";
    }

    if(userAchievement != Achievement_Any){
        request += " AND (ACHIEVEMENT = " + QString::number(userAchievement) + ")";
    }

    if(authorizedNow != AuthorizedStatus_Any){
        request += " AND (AUTHORIZED = " + QString::number(authorizedNow) + ")";
    }

    if(nickname != ""){
        request += " AND (CONTAINS(NICKNAME, \'" + nickname + "\'))";
    }

    request += " AND (LOGIN_DATE_TIME >= \'" + loginAfter.toString(SQL_DATETIME_FORMAT) + "\')";
    request += " AND (LOGIN_DATE_TIME <= \'" + loginBefore.toString(SQL_DATETIME_FORMAT) + "\')";
    request += ")";

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        return get_query_users(query);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<User>();
        }
        }
    }
}

MafiaList<User> UserDatabaseManager::get_query_users(QSqlQuery *query)
{
    MafiaList<User> users = MafiaList<User>();

    QSqlRecord record = query->record();
    while (query->next()) {
        User current = User();
        current.id = query_value_to_variable<UserIdType>(query->value(record.indexOf("ID")));
        current.salt = query_value_to_variable<QString>(query->value(record.indexOf("PASSWIRD_SALT")));
        current.chats = query_value_to_variable<MafiaList<ChatIdType>>(query->value(record.indexOf("ALL_CHATS")));
        current.email = query_value_to_variable<QString>(query->value(record.indexOf("EMAIL")));
        current.allGames = query_value_to_variable<MafiaList<GameIdType>>(query->value(record.indexOf("ALL_GAMES")));
        current.nickname = query_value_to_variable<QString>(query->value(record.indexOf("NICKNAME")));
        current.authorized = query_value_to_variable<AuthorizedStatus>(query->value(record.indexOf("IS_AUTHORIZED")));
        current.accountType = query_value_to_variable<AccountType>(query->value(record.indexOf("ACCOUNT_TYPE")));
        current.achievement = query_value_to_variable<Achievement>(query->value(record.indexOf("ACHIEVEMENT")));
        current.isConfirmed = query_value_to_variable<Status>(query->value(record.indexOf("IS_CONFIRMED")));
        current.passwordHash = query_value_to_variable<QString>(query->value(record.indexOf("PASSWORD_HASH")));
        current.transactions = query_value_to_variable<MafiaList<TransactionIdType>>(query->value(record.indexOf("TRANSACTIONS")));
        current.loginDateTime = query_value_to_variable<QDateTime>(query->value(record.indexOf("LOGIN_DATE_TIME")));
        current.confirmationKey = query_value_to_variable<QString>(query->value(record.indexOf("CONFIRMATION_KEY")));
        current.defeatesByRoles = query_value_to_variable<MafiaList<int>>(query->value(record.indexOf("DEFEATES_BY_ROLES")));
        current.victoriesByRoles = query_value_to_variable<MafiaList<int>>(query->value(record.indexOf("VICTORIES_BY_ROLES")));

        users.append(current);
    }

    return users;
}

UserIdType UserDatabaseManager::add_user(User user)
{
    QString request = "INSERT INTO " + dbName + "(ID, NICKNAME, EMAIL, PASSWORD_HASH, PASSWORD_SALT, IS_CONFIRMED, IS_AUTHORIZED, "
                                                "ACCOUNT_TYPE, LOGIN_DATE_TIME, ACHIEVEMENT, CONFIRMATION_KEY, TRANSACTIONS, "
                                                "ALL_GAMES, VICTORIES_BY_ROLES, DEFEATES_BY_ROLES, ALL_CHATS)"
                                                "VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9, %11, %12, %13, %14, %15, %16, %17)";

    QString userIdRequest = "SELECT MAX(ID) FROM " + dbName;
    try {
        QSqlQuery* userIdQuery = dbWorker->run_query(userIdRequest);

        if(userIdQuery->next()){
            user.id = query_value_to_variable<UserIdType>(userIdQuery->value(0)) + 1;
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
    request = request.arg("\'" + user.loginDateTime.toString(SQL_DATETIME_FORMAT) + "\'");
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

QString UserDatabaseManager::generate_confirmation_key()
{
    const int confirmationKeySize = 20;

    QString confirmationKey = "";

    for(int i = 0; i < confirmationKeySize; i++){
        confirmationKey += (char)((char)'A' + (char)(qrand() % 26));
    }

    return confirmationKey;
}

void UserDatabaseManager::register_game(Gameplay::Game game)
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


void UserDatabaseManager::register_game(UserIdType userId, GameIdType gameId, Gameplay::Role role, Gameplay::GamePersonalResult result)
{
    QString allGamesRequest = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
        QSqlQuery* allGamesQuery = dbWorker->run_query(allGamesRequest);
        QSqlRecord allGamesRecord = allGamesQuery->record();
        if(allGamesQuery->next()){
            MafiaList<GameIdType> games = query_value_to_variable<MafiaList<GameIdType>>(allGamesQuery->value(allGamesRecord.indexOf("ALL_GAMES")));
            games.append(gameId);

            QString request = "UPDATE " + dbName +
                    "\nSET ALL_GAMES = \'" + QString::fromStdString(qbytearray_from_qlist<GameIdType>(games).toStdString()) +
                    "\'\nWHERE (ID = " + QString::number(userId) + ")";

            dbWorker->run_query(request);



            switch (result) {
            case Gameplay::GamePersonalResult_Defeat:{
                MafiaList<int> defs = query_value_to_variable<MafiaList<int>>(allGamesQuery->value(allGamesRecord.indexOf("DEFEATES_BY_ROLES")));
                defs[role]++;

                QString defsRequest = "UPDATE " + dbName +
                        "\nSET DEFEATES_BY_ROLES = \'" + QString::fromStdString(qbytearray_from_qlist<int>(defs).toStdString()) +
                        "\'\nWHERE (ID = " + QString::number(userId) + ")";

                dbWorker->run_query(defsRequest);
                break;
            }
            case Gameplay::GamePersonalResult_Victory:{
                MafiaList<int> victs = query_value_to_variable<MafiaList<int>>(allGamesQuery->value(allGamesRecord.indexOf("VICTORIES_BY_ROLES")));
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
