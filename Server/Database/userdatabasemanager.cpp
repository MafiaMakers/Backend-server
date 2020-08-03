#include "userdatabasemanager.h"
#include "Exceptions/databaseworkingexception.h"
#include "user.h"
#include "System/SHA_256.h"
#include "Gameplay/game_s.h"
#include <iostream>
#include "System/keygen.h"
using namespace Mafia;
using namespace Database;


//const QString UserDatabaseWorker::dbName = "users_db";

UserDatabaseManager::UserDatabaseManager(DatabaseWorker* _dbWorker) : DatabaseManager(_dbWorker, "users_db")
{
    try {
		//Пробуем получить значения. Если БД еще не была создана,
		//будет выброшена ошибка и мы ее обработаем.
		//Если БД уже была создана, то ошибки не будет и мы ничего не будем делать
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            try {
				//Создаем новую таблицу
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
			exception->close();
            break;
        }
		//Если ошибка другого типа, то все плохо...
        default:{
            exception->show();
        }
        }
    }
}

UserIdType UserDatabaseManager::add_user(QString nickname, QString email, QString password, QString &confirmationKey)
{
	//Для начала создаем пустого пользователя, которого потом заполним необходимыми значениями
    User newUser = User();
	//Заполняем пользователя значениями
    newUser.nickname = nickname;
    newUser.email = email;
    newUser.salt = System::KeyGen::generate_key<QString>(SALT_SIZE);

	//Собираем строку для хеширования пароля
    QString addedPassword = password + newUser.salt + localParameter;

    std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());

    newUser.passwordHash = QString::fromStdString(hash);

	//Далее заполняем поля 'значениями по умолчанию'
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
    newUser.confirmationKey = System::KeyGen::generate_key<QString>(CONFIRMATION_KEY_SIZE);

    confirmationKey = newUser.confirmationKey;

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
		//Собираем строку, которую следует захешировать для сверения с хешем пароля
        QString addedPassword = password + user.salt + localParameter;

        std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());
		//Сверяем хеши пароля
        if(user.passwordHash == QString::fromStdString(hash)){
			QString request = "UPDATE " + dbName +
					"\nSET IS_AUTHORIZED = " + QString::number(AuthorizedStatus_Authorized) +
					"\nWHERE (ID =" + QString::number(id) + " )";
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
	//Чтобы проверить, что такой пользователь вообще существует, попробуем у него взять email
	QString emailRequest = "SELECT EMAIL FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

	QString request = "UPDATE " + dbName +
			"\nSET IS_AUTHORIZED = " + QString::number(AuthorizedStatus_NotAuthorized) +
			"\nWHERE (ID =" + QString::number(id) + " )";
	try {
		//Если пользователь существует, то все ок, продолжаем. Если нет, то будет исключение
		QSqlQuery emailQuery = dbWorker->run_query(emailRequest);
		if(!emailQuery.next()){
			throw Exceptions::Exception::generate(System::String("No such ID"),
				Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
		}

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
		//Получаем ключ подтверждения email-а
        QString request = "SELECT CONFIRMATION_KEY FROM \'" + dbName + "\' WHERE (ID = " + QString::number(id) + ")";
		QSqlQuery query = dbWorker->run_query(request);
		if(query.next()){
			QString realKey = query_value_to_variable<QString>(query.value(0));

			bool matches = realKey == confirmationKey;
			//Если ключи совпадают, то изменяем статус пользователя
			if(matches){
				QString updateRequest = "UPDATE " + dbName +
						"\nSET IS_CONFIRMED = " + QString::number(Status_Confirmed) +
						"\nWHERE (ID = " + QString::number(id) + ")";
				dbWorker->run_query(updateRequest);
			}

			return matches;
        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
		//Получаем из БД все данные о пользователе
        QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

		QSqlQuery query = dbWorker->run_query(request);

		QSqlRecord record = query.record();

		if(query.next()){
			//Создаем пустого пользователя и заполняем данными из запроса
            User currentUser = User();

            currentUser.id = id;
			currentUser.salt = query_value_to_variable<QString>(query.value(record.indexOf("PASSWORD_SALT")));

			currentUser.chats = query_value_to_variable<MafiaList<ChatIdType>>(query.value(record.indexOf("ALL_CHATS")));

			currentUser.email = query_value_to_variable<QString>(query.value(record.indexOf("EMAIL")));
			currentUser.allGames = query_value_to_variable<MafiaList<GameIdType>>(query.value(record.indexOf("ALL_GAMES")));
			currentUser.nickname = query_value_to_variable<QString>(query.value(record.indexOf("NICKNAME")));

			currentUser.authorized = query_value_to_variable<AuthorizedStatus>(query.value(record.indexOf("IS_AUTHORIZED")));

			currentUser.accountType = query_value_to_variable<AccountType>(query.value(record.indexOf("ACCOUNT_TYPE")));
			currentUser.achievement = query_value_to_variable<Achievement>(query.value(record.indexOf("ACHIEVEMENT")));
			currentUser.isConfirmed = query_value_to_variable<Status>(query.value(record.indexOf("IS_CONFIRMED")));
			currentUser.passwordHash = query_value_to_variable<QString>(query.value(record.indexOf("PASSWORD_HASH")));

            currentUser.transactions = query_value_to_variable<MafiaList<TransactionIdType>>(
																					query.value(record.indexOf("TRANSACTIONS")));
			currentUser.loginDateTime = query_value_to_variable<QDateTime>(query.value(record.indexOf("LOGIN_DATE_TIME")));

            currentUser.confirmationKey = query_value_to_variable<QString>(
																				query.value(record.indexOf("CONFIRMATION_KEY")));
            currentUser.defeatesByRoles = query_value_to_variable<MafiaList<int>>(
																				query.value(record.indexOf("DEFEATES_BY_ROLES")));
            currentUser.victoriesByRoles = query_value_to_variable<MafiaList<int>>(
																				query.value(record.indexOf("VICTORIES_BY_ROLES")));

            return currentUser;
        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
		//Получаем список транзакций пользователя (он зашифрован в строку)
        QString request = "SELECT TRANSACTIONS FROM " + dbName + " WHERE (ID = " + QString::number(user) + ")";

		QSqlQuery query = dbWorker->run_query(request);

		if(query.next()){
			//Дешифруем строку в список и добавляем в него необходимую транзакцию
			MafiaList<TransactionIdType> allTransactions = query_value_to_variable<MafiaList<TransactionIdType>>(query.value(0));
            allTransactions.append(transaction);
			//Шифруем обратно в строку
            QByteArray transactionsData = qbytearray_from_qlist<TransactionIdType>(allTransactions);
			//Изменяем значение в БД
            QString passRequest = "UPDATE " + dbName +
                    "\nSET TRANSACTIONS = \'" + QString::fromStdString(transactionsData.toStdString()) +
                    "\'\nWHERE (ID = " + QString::number(user) + ")";

            dbWorker->run_query(passRequest);

        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
	//Чтобы проверить, что такой пользователь вообще существует, попробуем взять его email
	QString emailRequest = "SELECT EMAIL FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
	QSqlQuery emailQuery = dbWorker->run_query(emailRequest);
	//Если не удастся, кинем ошибку
	if(!emailQuery.next()){
		throw Exceptions::Exception::generate(System::String("No such ID"),
			Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
	}

	try {
		//Для изменения своего пароля надо быть авторизованным (хах, ну вроде логично)
		QString authorizedRequest = "SELECT IS_AUTHORIZED FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
		QSqlQuery authoizedQuery = dbWorker->run_query(authorizedRequest);
		if(authoizedQuery.next()){
			//Проверим, что пользователь авторизован. Иначе кинем ошибку
			if(query_value_to_variable<AuthorizedStatus>(authoizedQuery.value(0)) == AuthorizedStatus_Authorized){
				reset_users_password(newPassword, id);
			} else{
				throw Exceptions::Exception::generate(System::String("User must be authorized to change password"),
															   Exceptions::DatabaseWorkingExceptionId_NotAuthorizedAction);
			}

		} else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
														   Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
	//Т.к. email - это primary key, по нему можно однозначно получить id
    QString selectRequest = "SELECT ID FROM " + UserDatabaseManager::dbName + " WHERE (EMAIL = \'" + email + "\')";

    try {
		//Отправляем запрос в БД
		QSqlQuery query = dbWorker->run_query(selectRequest);

		if(query.next()){
			//Берем id из запроса
			UserIdType id = query_value_to_variable<UserIdType>(query.value(0));
            return id;
        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
	//Чтобы проверить, что пользователь вообще существует, попробуем взять его email
	QString emailRequest = "SELECT EMAIL FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
	QSqlQuery emailQuery = dbWorker->run_query(emailRequest);

	//Если не удается, то кинем исключение
	if(!emailQuery.next()){
		throw Exceptions::Exception::generate(System::String("No such ID"),
			Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
	}

	//Изменяем значение достижения в БД
	QString request = "UPDATE " + dbName + "\nSET ACHIEVEMENT = " + QString::number(achievement) +
			"\nWHERE (ID = " + QString::number(id) + ")";

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
	//Отправляем запрос в БД
    QString chatsRequest = "SELECT ALL_CHATS FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
		QSqlQuery chatsQuery = dbWorker->run_query(chatsRequest);

		if(chatsQuery.next()){
			//Дешифруем строку из запроса в список чатов
			MafiaList<ChatIdType> chats = query_value_to_variable<MafiaList<ChatIdType>>(chatsQuery.value(0));
			//Добавляем новый чат
            chats.append(chatId);
			//Шифруем обратно в список
            QByteArray newChats = qbytearray_from_qlist<ChatIdType>(chats);
			//Изменяем БД
            QString request = "UPDATE " + dbName + "\nSET ALL_CHATS = \'" + QString::fromStdString(newChats.toStdString()) +
                    "\'\n WHERE (ID = " + QString::number(userId) + ")";
            dbWorker->run_query(request);
        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
	//Отправляем запрос в БД
    QString chatsRequest = "SELECT ALL_CHATS FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
		QSqlQuery chatsQuery = dbWorker->run_query(chatsRequest);

		if(chatsQuery.next()){
			//Дешифруем результат запроса в список
			MafiaList<ChatIdType> chats = query_value_to_variable<MafiaList<ChatIdType>>(chatsQuery.value(0));
			//Пользователь должен состоять в чате, чтобы его можно было оттуда удалить
            if(chats.contains(chatId)){
				//Удаляем из списка и шифруем список обратно в строку
                chats.removeOne(chatId);
                QByteArray newChats = qbytearray_from_qlist<ChatIdType>(chats);
				//Изменяем БД
                QString request = "UPDATE " + dbName + "\nSET ALL_CHATS = \'" + QString::fromStdString(newChats.toStdString()) +
                        "\'\n WHERE (ID = " + QString::number(userId) + ")";
                dbWorker->run_query(request);
            } else{
				throw Exceptions::Exception::generate(System::String("This user is not in this chat"),
                                                               Exceptions::DatabaseWorkingExceptionId_DeleteMissedItemAttempt);
            }

        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
	//Проверяем, что пользователь существует. Для этого берем его email
	QString emailRequest = "SELECT EMAIL FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
	QSqlQuery emailQuery = dbWorker->run_query(emailRequest);
	//Если email не нашелся, то кидаем исключение
	if(!emailQuery.next()){
		throw Exceptions::Exception::generate(System::String("No such ID"),
			Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
	}

    try {
		//Изменяем тип аккаунта
        QString request = "UPDATE " + dbName + "\nSET ACCOUNT_TYPE = " + QString::number(newAccountType) + "\nWHERE (ID = " +
                QString::number(id) + ")";
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
	//Получаем список всех email-ов всех пользователей
    QString getRequest = "SELECT EMAIL FROM " + dbName;

    try {
		//Получаем email нашего пользователя, чтобы
		//1. убедиться, что он существует 2. не учитывать его email в списке других
		QString idRequest = "SELECT EMAIL FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
		QSqlQuery idQuery = dbWorker->run_query(idRequest);
		QString myEmail = "";
		//Если нашли его email, то запомним его.
		//Если не нашли, то кинем исключение
		if(idQuery.next()){
			myEmail = query_value_to_variable<QString>(idQuery.value(0));
		} else {
			throw Exceptions::Exception::generate(System::String("No such ID"),
				Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
		}

		QSqlQuery getQuery = dbWorker->run_query(getRequest);

		//Пройдемся по email-ам всех пользователей и проверим, что новый email еще ни кем не занят
		while(getQuery.next()){
			QString currentEmail = query_value_to_variable<QString>(getQuery.value(0));
			if(currentEmail == newEmail && currentEmail != myEmail){
                return false;
            }
        }

		//Если же все ок, то изменим email.
		QString setRequest = "UPDATE " + dbName +
				"\nSET EMAIL = \'" + newEmail +
				"\'\nWHERE (ID = " + QString::number(id) + ")";

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
	//Чтобы убедиться, что пользователь существует, возьмем его email
	QString emailRequest = "SELECT EMAIL FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
	QSqlQuery emailQuery = dbWorker->run_query(emailRequest);
	//Если такого пользователя нет, кинем исключение
	if(!emailQuery.next()){
		throw Exceptions::Exception::generate(System::String("No such ID"),
			Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
	}
	//Изменим имя пользователя в БД
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

    QString request = "SELECT * FROM " + dbName +
            create_filter_request(ids, userStatus, userAchievement, authorizedNow, nickname, loginAfter, loginBefore);
    try {
		QSqlQuery query = dbWorker->run_query(request);

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

MafiaList<UserIdType> UserDatabaseManager::get_users_ids(MafiaList<UserIdType> ids, Status userStatus, Achievement userAchievement,
                                                         AuthorizedStatus authorizedNow, QString nickname,
                                                         QDateTime loginAfter, QDateTime loginBefore)
{
    QString request = "SELECT ID FROM " + dbName +
            create_filter_request(ids, userStatus, userAchievement, authorizedNow, nickname, loginAfter, loginBefore);

    MafiaList<UserIdType> userIds = MafiaList<UserIdType>();

    try {
		QSqlQuery query = dbWorker->run_query(request);

		while (query.next()) {
			userIds.append(query_value_to_variable<UserIdType>(query.value(0)));
        }
        return userIds;
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<UserIdType>();
        }
        }
    }
}

void UserDatabaseManager::reset_users_password(QString newPassword, UserIdType id)
{
	//Для начала получим соль пользователя, чтобы позже создать хеш
    QString saltRequest = "SELECT PASSWORD_SALT FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";
	QSqlQuery saltQuery = dbWorker->run_query(saltRequest);
	//Если пользователь нашелся, меняем его пароль. Если нет - кидаем исключение
	if(saltQuery.next()){

		QString salt = query_value_to_variable<QString>(saltQuery.value(0));
		//Заново создаем строку для хеширования
        QString addedPassword = newPassword + salt + localParameter;
		//И хешируем
        std::string hash = System::SHA256().hash(addedPassword.toStdString().c_str());

        QString passwordHash = QString::fromStdString(hash);
		//Изменяем БД
        QString request = "UPDATE " + dbName + "\nSET PASSWORD_HASH = \'" + passwordHash +
                "\'\nWHERE (ID = " + QString::number(id) + ")";
        dbWorker->run_query(request);

    } else{
		throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                       Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
    }
}

QString UserDatabaseManager::create_filter_request(MafiaList<UserIdType> ids,
												   Status userStatus,
												   Achievement userAchievement,
												   AuthorizedStatus authorizedNow,
												   QString nickname,
												   QDateTime loginAfter,
												   QDateTime loginBefore)
{
	//Собираем условие для запроса, которое выбирает пользователей указанными фильтрами
    QString request = " WHERE (TRUE";

	//Сначала проверяем список id (если пустой, то подходят все id)
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

	//Затем проверяем статус
    if(userStatus != Status_Any){
        request += " AND (IS_CONFIRMED = " + QString::number(userStatus) + ")";
    }

	//Достижение
    if(userAchievement != Achievement_Any){
        request += " AND (ACHIEVEMENT = " + QString::number(userAchievement) + ")";
    }

	//Авторизованность
    if(authorizedNow != AuthorizedStatus_Any){
		request += " AND (IS_AUTHORIZED = " + QString::number(authorizedNow) + ")";
    }

	//Имя (содержит ли что-то)
    if(nickname != ""){
		request += " AND (NICKNAME LIKE \'%" + nickname + "%\')";
    }

	//И время и дату регистрации
    request += " AND (LOGIN_DATE_TIME >= \'" + loginAfter.toString(SQL_DATETIME_FORMAT) + "\')";
    request += " AND (LOGIN_DATE_TIME <= \'" + loginBefore.toString(SQL_DATETIME_FORMAT) + "\')";
    request += ")";

    return request;
}

MafiaList<User> UserDatabaseManager::get_query_users(QSqlQuery query)
{
	//Создаем пустой список, который потом заполним по одному пользователю
    MafiaList<User> users = MafiaList<User>();

	QSqlRecord record = query.record();
	while (query.next()) {
		//Заполняем пользователя значениями из запроса
        User current = User();
		current.id = query_value_to_variable<UserIdType>(query.value(record.indexOf("ID")));
		current.salt = query_value_to_variable<QString>(query.value(record.indexOf("PASSWORD_SALT")));
		current.chats = query_value_to_variable<MafiaList<ChatIdType>>(query.value(record.indexOf("ALL_CHATS")));
		current.email = query_value_to_variable<QString>(query.value(record.indexOf("EMAIL")));
		current.allGames = query_value_to_variable<MafiaList<GameIdType>>(query.value(record.indexOf("ALL_GAMES")));
		current.nickname = query_value_to_variable<QString>(query.value(record.indexOf("NICKNAME")));
		current.authorized = query_value_to_variable<AuthorizedStatus>(query.value(record.indexOf("IS_AUTHORIZED")));
		current.accountType = query_value_to_variable<AccountType>(query.value(record.indexOf("ACCOUNT_TYPE")));
		current.achievement = query_value_to_variable<Achievement>(query.value(record.indexOf("ACHIEVEMENT")));
		current.isConfirmed = query_value_to_variable<Status>(query.value(record.indexOf("IS_CONFIRMED")));
		current.passwordHash = query_value_to_variable<QString>(query.value(record.indexOf("PASSWORD_HASH")));
		current.transactions = query_value_to_variable<MafiaList<TransactionIdType>>(query.value(record.indexOf("TRANSACTIONS")));
		current.loginDateTime = query_value_to_variable<QDateTime>(query.value(record.indexOf("LOGIN_DATE_TIME")));
		current.confirmationKey = query_value_to_variable<QString>(query.value(record.indexOf("CONFIRMATION_KEY")));
		current.defeatesByRoles = query_value_to_variable<MafiaList<int>>(query.value(record.indexOf("DEFEATES_BY_ROLES")));
		current.victoriesByRoles = query_value_to_variable<MafiaList<int>>(query.value(record.indexOf("VICTORIES_BY_ROLES")));
		//Добавляем получившегося пользователя в список
        users.append(current);
    }

    return users;
}

UserIdType UserDatabaseManager::add_user(User user)
{
	//Создаем шаблон запроса, который потом заполним данными (эти '%...')
    QString request = "INSERT INTO " + dbName + "(ID, NICKNAME, EMAIL, PASSWORD_HASH, PASSWORD_SALT, IS_CONFIRMED, IS_AUTHORIZED, "
                                                "ACCOUNT_TYPE, LOGIN_DATE_TIME, ACHIEVEMENT, CONFIRMATION_KEY, TRANSACTIONS, "
                                                "ALL_GAMES, VICTORIES_BY_ROLES, DEFEATES_BY_ROLES, ALL_CHATS)"
                                                "VALUES (%1, %2, %3, %4, %5, %6, %7, %8, %9, %11, %12, %13, %14, %15, %16, %17)";

	//Получаем максимальный id из БД
    QString userIdRequest = "SELECT MAX(ID) FROM " + dbName;
    try {
		QSqlQuery userIdQuery = dbWorker->run_query(userIdRequest);

		if(userIdQuery.next()){
			//Задаем пользователю соответствующий id
			user.id = query_value_to_variable<UserIdType>(userIdQuery.value(0)) + 1;
        } else{
			throw Exceptions::Exception::generate(System::String("Request answer was null"),
                                                           Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }

	//Заполняем значениями шаблон запроса
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
		//Выполняем запрос
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


void UserDatabaseManager::register_game(Gameplay::Game game)
{
    try {
        for(int i = 0; i < game.users.length(); i++){
			//Для каждого участника игры считаем его личный результат и добавляем ему лично
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


void UserDatabaseManager::register_game(UserIdType userId, GameIdType gameId, Gameplay::Role role,
                                        Gameplay::GamePersonalResult result)
{
	//Для начала получим все сведения о пользователе
    QString allGamesRequest = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(userId) + ")";
    try {
		QSqlQuery allGamesQuery = dbWorker->run_query(allGamesRequest);
		QSqlRecord allGamesRecord = allGamesQuery.record();
		if(allGamesQuery.next()){
			//Дешифруем список игр пользователя
            MafiaList<GameIdType> games = query_value_to_variable<MafiaList<GameIdType>>(
																		allGamesQuery.value(allGamesRecord.indexOf("ALL_GAMES")));
			//Добавляем в список эту игру
			games.append(gameId);
			//Изменяем список игр
            QString request = "UPDATE " + dbName +
                    "\nSET ALL_GAMES = \'" + QString::fromStdString(qbytearray_from_qlist<GameIdType>(games).toStdString()) +
                    "\'\nWHERE (ID = " + QString::number(userId) + ")";

            dbWorker->run_query(request);


			//В зависимости от персонального результата для игрока изменяем либо victoriesByRoles либо defeatesByRoles
            switch (result) {
				case Gameplay::GamePersonalResult_Defeat:{
					//Дешифруем список
					MafiaList<int> defs = query_value_to_variable<MafiaList<int>>(allGamesQuery.value(
																					  allGamesRecord.indexOf("DEFEATES_BY_ROLES")));
					//Изменяем соответствующую ячейку
					defs[role]++;
					//Изменяем БД
					QString defsRequest = "UPDATE " + dbName +
							"\nSET DEFEATES_BY_ROLES = \'" + QString::fromStdString(qbytearray_from_qlist<int>(defs).toStdString()) +
							"\'\nWHERE (ID = " + QString::number(userId) + ")";

					dbWorker->run_query(defsRequest);
					break;
				}
				case Gameplay::GamePersonalResult_Victory:{
					//Дешифруем список из строки
					MafiaList<int> victs = query_value_to_variable<MafiaList<int>>(allGamesQuery.value(
																					   allGamesRecord.indexOf("VICTORIES_BY_ROLES")));
					//Изменяем соответствующую ячейку
					victs[role]++;
					//Изменяем БД
					QString victRequest = "UPDATE " + dbName +
							"\nSET VICTORIES_BY_ROLES = \'" + QString::fromStdString(qbytearray_from_qlist<int>(victs).toStdString()) +
							"\'\nWHERE (ID = " + QString::number(userId) + ")";

					dbWorker->run_query(victRequest);
					break;
				}
			default:{
					//Иначе ничего не делаем, т.к. пользователь не участвовал активно в игре (был наблюдателем)
				}
            }

        } else{
			throw Exceptions::Exception::generate(System::String("No such ID"),
					Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
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
