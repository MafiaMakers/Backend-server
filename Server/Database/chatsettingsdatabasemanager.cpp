#include "chatsettingsdatabasemanager.h"
#include "databasehelper.h"
using namespace Mafia;
using namespace Database;

ChatSettingsDatabaseManager::ChatSettingsDatabaseManager(DatabaseWorker *databaseWorker) : DatabaseManager(databaseWorker, "chats_db")
{
    try {
		//Если БД уже была создана, то ничего не произойдет, а если не была, то кинет ошибку
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
			//Если ошибка, связанная с SQL, то БД еще не создана и ее стоит создать
			case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
				QString request = "CREATE TABLE " + dbName + "("
						"ID " + get_sql_type<ChatIdType>() + " PRIMARY KEY" + NOT_NULL + ", "
						"USERS " + get_sql_type<MafiaList<UserIdType>>(MAX_USERS_IN_CHAT) + ", "
						"CAPABILITIES " + get_sql_type<MafiaList<ChatCapability>>(MAX_USERS_IN_CHAT) + ", "
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
				exception->close();
				break;
			}
			//Если это какая-то другая ошибка, то все плохо...
			default:{
				exception->show();
				break;
			}
        }
    }
}

ChatIdType ChatSettingsDatabaseManager::create_chat()
{
	//Сначала узнаем максимальный ID в БД
    QString idRequest = "SELECT COALESCE(MAX(ID), 0) FROM " + dbName;

    try {
		QSqlQuery idQuery = dbWorker->run_query(idRequest);
		//Результат точно должен быть, т.к. использовался COALESCE
		if(idQuery.next()){
			//Задаем чату нужный ID
			ChatIdType id = query_value_to_variable<ChatIdType>(idQuery.value(0)) + 1;
            QString request = "INSERT INTO " + dbName + " (ID, TIMESTAMP, USERS, CAPABILITIES) VALUES (%1, %2, \'\', \'\')";
			//Заполняем пропуски в запросе
            request = request.arg(QString::number(id));
            request = request.arg("\'" + QDateTime::currentDateTimeUtc().toString(SQL_DATETIME_FORMAT) + "\'");

            dbWorker->run_query(request);
            return id;
		//Но если результата все же нет, то что-то не так...
        } else{
			throw Exceptions::Exception::generate(System::String("Null query answer"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
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
	//Берем списки пользователей и их ролей в чате (они зашифрованы в строки), чтобы потом в них добавить еще одного пользователя
    QString chatRequest = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
		QSqlQuery chatQuery = dbWorker->run_query(chatRequest);

		if(chatQuery.next()){
			QSqlRecord record = chatQuery.record();
			//Дешифруем строки в списки
			MafiaList<UserIdType> users =
					query_value_to_variable<MafiaList<UserIdType>>(chatQuery.value(record.indexOf("USERS")));
			MafiaList<ChatCapability> capabilities =
					query_value_to_variable<MafiaList<ChatCapability>>(chatQuery.value(record.indexOf("CAPABILITIES")));

            if(users.contains(user)){
				throw Exceptions::Exception::generate(System::String("Attemp to add user to chat with this user"),
                                                               Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt);
            }

            capabilities.append(usersCapability);
            users.append(user);
			//Шфируем обратно и изменяем БД
            QString request = "UPDATE " + dbName + "\nSET USERS = %1, CAPABILITIES = %2 WHERE (ID = " + QString::number(chat) + ")";
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(users).toStdString()) + "\'");
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<ChatCapability>(capabilities).toStdString()) + "\'");

            dbWorker->run_query(request);
        } else{
			throw Exceptions::Exception::generate(System::String("Null sql answer!"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
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
	//Собираем условие по частям
    QString request = "SELECT ID FROM " + dbName + " WHERE (TRUE";

	request += generate_request_id(ids);

	request += generate_request_user(users, usersFilter);

	request += " AND (TIMESTAMP >= \'" + createdAfter.toString(SQL_DATETIME_FORMAT) + "\')";
	request += " AND (TIMESTAMP <= \'" + createdBefore.toString(SQL_DATETIME_FORMAT) + "\')";
    request += ")";

    try {
		QSqlQuery query = dbWorker->run_query(request);

        MafiaList<ChatIdType> result = MafiaList<ChatIdType>();
		//Вытаскиваем все значения из запроса
		while(query.next()){
			result.append(query_value_to_variable<ChatIdType>(query.value(0)));
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
	//Берем списки пользователей и их ролей в чате (чтобы знать, какому пользователю какая роль соответствует)
    QString chatRequest = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
		QSqlQuery chatQuery = dbWorker->run_query(chatRequest);

		if(chatQuery.next()){
			//Дешифруем строки в списки
			QSqlRecord record = chatQuery.record();
			MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(chatQuery.value(record.indexOf("USERS")));
			MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(chatQuery.value(record.indexOf("CAPABILITIES")));
			//Пользователь должен находиться в этом чате
			if(users.contains(user)){
                capabilities[users.indexOf(user)] = newCapability;
            } else{
				throw Exceptions::Exception::generate(System::String("Attemp to edit users capability in chat without this user"),
                                                               Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt);
            }

			//Шифруем обратно в строки и меняем БД
            QString request = "UPDATE " + dbName + "\nSET CAPABILITIES = %1 WHERE (ID = " + QString::number(chat) + ")";
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<ChatCapability>(capabilities).toStdString()) + "\'");

            dbWorker->run_query(request);
        } else{
			throw Exceptions::Exception::generate(System::String("Null sql answer!"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
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
	//Берем списки пользователей и их ролей
    QString chatRequest = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
		QSqlQuery chatQuery = dbWorker->run_query(chatRequest);

		if(chatQuery.next()){
			//Дещифруем их в списки
			QSqlRecord record = chatQuery.record();
			MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(chatQuery.value(record.indexOf("USERS")));
			MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(chatQuery.value(record.indexOf("CAPABILITIES")));
			//Пользователь должен быть в этом чате
			if(users.contains(user)){
				//Удаляем его
                capabilities.removeAt(users.indexOf(user));
                users.removeOne(user);
            } else{
				throw Exceptions::Exception::generate(System::String("Attemp to remove user from chat without this user"),
                                                               Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt);
            }


			//Шифруем обратно и изменяем БД
            QString request = "UPDATE " + dbName + "\nSET USERS = %1, CAPABILITIES = %2 WHERE (ID = " + QString::number(chat) + ")";
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(users).toStdString()) + "\'");
            request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<ChatCapability>(capabilities).toStdString()) + "\'");

            dbWorker->run_query(request);
        } else{
			throw Exceptions::Exception::generate(System::String("Null sql answer!"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
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
		QSqlQuery query = dbWorker->run_query(request);
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
	//Чтобы узнать, что пользователь есть в данном чате, надо, чтобы строка,
	//которая представляет собой зашифрованного этого пользователя содержалась
	//в строке со всеми пользователями этого чата
    MafiaList<UserIdType> oneUser = MafiaList<UserIdType>() << user;
    QString oneUserString = QString::fromStdString(qbytearray_from_qlist<UserIdType>(oneUser).toStdString());

	QString request = "SELECT * FROM " + dbName + " WHERE (USERS LIKE \'%" + oneUserString + "%\')";
    try {
		QSqlQuery query = dbWorker->run_query(request);
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
		//Берем чаты, у которых нужный ID
		QSqlQuery query = dbWorker->run_query(request);
        MafiaList<Chat> chats = get_request_chat(query);
		//Такой должен быть только 1 (т.к. ID - PRIMARY KEY)
		if(chats.length() == 1){
            return chats[0];
        } else{
            if(chats.length() < 1){
				throw Exceptions::Exception::generate(System::String("No chat with this ID"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
            } else {
				throw Exceptions::Exception::generate(System::String("Something impossible happened! Many chats with same ID"),
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
			throw Exceptions::Exception::generate(System::String("Unknown chat capability"),
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
			throw Exceptions::Exception::generate(System::String("Unknown chat capability"),
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
			throw Exceptions::Exception::generate(System::String("Unknown chat capability"),
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
			throw Exceptions::Exception::generate(System::String("Unknown chat capability"),
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
	//Берем список пользователей и список их ролей в чате
    QString request = "SELECT USERS, CAPABILITIES FROM " + dbName + " WHERE (ID = " + QString::number(chat) + ")";

    try {
		QSqlQuery query = dbWorker->run_query(request);

		if(query.next()){
			//Дешифруем их из строк в списки
			MafiaList<UserIdType> users = query_value_to_variable<MafiaList<UserIdType>>(query.value(query.record().indexOf("USERS")));
            MafiaList<ChatCapability> capabilities = query_value_to_variable<MafiaList<ChatCapability>>(
													 query.value(query.record().indexOf("CAPABILITIES")));
			//Пользователь должен быть в этом чате
            if(users.contains(user)){
                return capabilities[users.indexOf(user)];
            } else{
                return ChatCapabilities_None;
            }
        } else{
			throw Exceptions::Exception::generate(System::String("Null query answer"),
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

QString ChatSettingsDatabaseManager::generate_request_id(MafiaList<ChatIdType> ids)
{
	//Собираем часть запроса, которая фильтрует по id
    QString request = "";
	//Если список пустой, то подходят любые id, тогда условие пустое
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

    return request;
}

QString ChatSettingsDatabaseManager::generate_request_user(MafiaList<UserIdType> users, FilterType filter)
{
	//Собираем условие запроса, фильтрующее по пользователям, которые есть в чате
    QString request = "";
	//Если список пустой, то подходят любые чаты и условие пустое
    if(users.length() > 0){
		request += " AND (";
        MafiaList<UserIdType> oneUserList = MafiaList<UserIdType>() << users[0];
        for(int i = 0; i < users.length(); i++){
            oneUserList[0] = users[i];
			//Если пользователь есть в чате, то строка с зашифрованным одним этим пользователем
			//будет подстрокой строки с зашифрованным списком пользователей
			request += "USERS LIKE \'%" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(oneUserList).toStdString()) + "%\'";

            if(i != users.length() - 1){
				//Тут есть 2 варианта: либо мы хотим, чтобы все пользователи из списка были в одном чате, либо
				//хотим, чтобы хоть кто-то из списка был в таком чате.
				//Для выбора нужного типа условия используется FilterType и get_sql_filter
                request += get_sql_filter(filter);

            } else{
                request += ")";
            }
        }
    }

    return request;
}

MafiaList<Chat> ChatSettingsDatabaseManager::get_request_chat(QSqlQuery query)
{
	//Будем по одному добавлять в пустой список все чаты
    MafiaList<Chat> chats = MafiaList<Chat>();

	QSqlRecord record = query.record();

	while (query.next()) {
        Chat current = Chat();
		//Заполняем пустой чат данными из запроса и добавляем его в список
		current.id = query_value_to_variable<ChatIdType>(query.value(record.indexOf("ID")));
		current.users = query_value_to_variable<MafiaList<UserIdType>>(query.value(record.indexOf("USERS")));
		current.creationTime = query_value_to_variable<QDateTime>(query.value(record.indexOf("TIMESTAMP")));
		current.usersCapabilities = query_value_to_variable<MafiaList<ChatCapability>>(query.value(record.indexOf("CAPABILITIES")));

        chats.append(current);
    }

    return chats;
}
