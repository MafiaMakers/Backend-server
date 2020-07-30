#include "gamedatabasemanager.h"
#include "Exceptions/databaseworkingexception.h"
#include "databasehelper.h"
#include "System/functions.h"
using namespace Mafia;
using namespace Database;

GameDatabaseManager::GameDatabaseManager(DatabaseWorker *databaseWorker) : DatabaseManager(databaseWorker, "games_db")
{
    try {
		//Если БД уже была создана, то функция ничего не сделает. Но если еще не была создана, то выкинет ошибку, которую мы обработаем.
        QString request = "SELECT * FROM " + dbName;
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            try {
				//Создаем БД
                QString createRequest = "CREATE TABLE " + dbName + " ("
                        "ID " + get_sql_type<GameIdType>() + " PRIMARY KEY " + NOT_NULL + ", "
                        "GAME_RESULT " + get_sql_type<Gameplay::GameResult>() + NOT_NULL + ", "
                        "PLAYERS " + get_sql_type<MafiaList<UserIdType>>(Gameplay::MAXIMUM_USERS_IN_GAME) + NOT_NULL + ", "
                        "ROLES " + get_sql_type<MafiaList<Gameplay::Role>>(Gameplay::MAXIMUM_USERS_IN_GAME) + NOT_NULL + ", "
                        "BEGIN " + get_sql_type<QDateTime>() + NOT_NULL + ", "
                        "END " + get_sql_type<QDateTime>() + NOT_NULL
						")";
                dbWorker->run_query(createRequest);
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
		//Если ошибка другого характера, то все плохо...
        default:{
            exception->show();
            break;
        }
        }
    }
}
Gameplay::Game GameDatabaseManager::get_game_data(GameIdType id)
{
    QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

    try {
		//Отправляем запрос в БД
        QSqlQuery* query = dbWorker->run_query(request);
        QSqlRecord record = query->record();
		//Если найден хоть один результат, то заполняем поля и возвращаем значения
        if(query->next()){
            Gameplay::Game game = Gameplay::Game();
            game.id = id;
            game.result = query_value_to_variable<Gameplay::GameResult>(query->value(record.indexOf("GAME_RESULT")));
            game.endingDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("END")));
            game.beginningDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("BEGIN")));
            game.roles = query_value_to_variable<MafiaList<Gameplay::Role>>(query->value(record.indexOf("ROLES")));
            game.users = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("PLAYERS")));

            return game;
		//Если результатов нет, то кидаем исключение
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"),
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

GameIdType GameDatabaseManager::add_game(Gameplay::Game &game)
{
	//Если количество ролей не соответствует количеству игроков, то что-то не так
    if(game.roles.length() != game.users.length()){
        throw new Exceptions::DatabaseWorkingException(System::String("Users and roles size mismatch"),
                                                       Exceptions::DatabaseWorkingExceptionId_ArraySizeMismatch);
    }

    try {
		//Получаем максимальный id в БД
        QString idRequest = "SELECT COALESCE(MAX(ID), 0) FROM " + dbName;
        QSqlQuery* idQuery = dbWorker->run_query(idRequest);
        if(idQuery->next()){
            game.id = query_value_to_variable<GameIdType>(idQuery->value(0)) + 1;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("NULL sql request answer"),
                                                           Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }

		//Создаем запрос с пустыми полями (эти вот '%...')
        QString request = "INSERT INTO " + dbName +
                " (ID, GAME_RESULT, ROLES, PLAYERS, BEGIN, END) VALUES (%1, %2, %3, %4, %5, %6)";
		//Заполняем пустые поля запроса
        request = request.arg(QString::number(game.id));
        request = request.arg(QString::number(game.result));
        request = request.arg("\'" + QString::fromStdString(
                                  qbytearray_from_qlist<Gameplay::Role>(game.roles).toStdString()) + "\'");
        request = request.arg("\'" + QString::fromStdString(
                                  qbytearray_from_qlist<UserIdType>(game.users).toStdString()) + "\'");
        request = request.arg("\'" + game.beginningDT.toString(SQL_DATETIME_FORMAT) + "\'");
        request = request.arg("\'" + game.endingDT.toString(SQL_DATETIME_FORMAT) + "\'");

		//Добавляем в БД новую строку и возвращаем id
        dbWorker->run_query(request);
        return game.id;

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }



}

MafiaList<Gameplay::Game> GameDatabaseManager::get_games_with(MafiaList<UserIdType> participants, FilterType participantsFilter,
                                                          MafiaList<Gameplay::Role> roles, FilterType rolesFilter,
                                                          MafiaList<Gameplay::GameResult> outcomes, QDateTime beginAfter,
                                                              QDateTime endBefore)
{
    try {
		//Собираем запрос по частям
        QString request = "SELECT * FROM " + dbName + " WHERE (TRUE";

		request += generate_request_participant(participants, participantsFilter);

		request += generate_request_outcomes(outcomes);

		request += generate_request_roles(roles, rolesFilter);

        request = request + " AND (BEGIN >= \'" + beginAfter.toString(SQL_DATETIME_FORMAT) + "\')";
        request = request + " AND (END <= \'" + endBefore.toString(SQL_DATETIME_FORMAT) + "\')";
        request += ")";

		//Выполняем запрос
        QSqlQuery* query = dbWorker->run_query(request);
        QSqlRecord record = query->record();

		//Создаем пустой список, чтобы потом его заполнить
        MafiaList<Gameplay::Game> games = MafiaList<Gameplay::Game>();
        while(query->next()){
			//Заполняем поля новой игры
            Gameplay::Game current = Gameplay::Game();

            current.id = query->value(record.indexOf("ID")).toInt();
            current.result = query_value_to_variable<Gameplay::GameResult>(query->value(record.indexOf("GAME_RESULT")));
            current.beginningDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("BEGIN")));
            current.endingDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("END")));
            current.roles = query_value_to_variable<MafiaList<Gameplay::Role>>(query->value(record.indexOf("ROLES")));
            current.users = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("PLAYERS")));

			//Добавляем игру в список
            games.append(current);
        }

        return games;
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<Gameplay::Game>();
        }
        }
    }

}

QString GameDatabaseManager::generate_request_participant(MafiaList<UserIdType> participants, FilterType filter)
{
	//Собираем часть условия запроса для фильтра по участникам игры
    QString request = "";

	//Если длина списка 0, то подходят любые игры
    if(participants.length() > 0){
        request = request + " AND (";
		//Обязательно должен быть установлен тип фильтра
        ASSERT((filter != FilterType_NONE), "Participants is not null but filter type is null");

        MafiaList<UserIdType> oneUser = MafiaList<UserIdType>() << 0;

        for(int i = 0; i < participants.length(); i++){
			//Если пользователь участвовал в игре, то строка с зашифрованным
			//этим пользователем будет подстрокой списка всех пользователей
            oneUser[0] = participants[i];
            QString user = QString::fromStdString(qbytearray_from_qlist<UserIdType>(oneUser).toStdString());
			request += "PLAYERS LIKE \'%" + user + "%\'";

            if(i != participants.length() - 1){
				//В зависимости от типа фильтра мы можем искать игры,
				//в которых участвовали все игроки из списка или хотя бы один
				request += get_sql_filter(filter);
            } else{
                request += ")";
            }
        }

    }

    return request;
}

QString GameDatabaseManager::generate_request_outcomes(MafiaList<Gameplay::GameResult> outcomes)
{
	//Собираем условие для запроса, фильтрующее игры по исходам
    QString request = "";

    if(outcomes.length() > 0){
        request = request + " AND (";
		//Ищем все игры, которые закончились как-либо из списка исходов
        for(int i = 0; i < outcomes.length(); i++){
            QString outcome = QString::number(outcomes[i]);
            request += "(GAME_RESULT = " + outcome + ")";
            if(i != outcomes.length() - 1){
                request += " OR ";
            } else{
                request += ")";
            }
        }
    }

    return request;
}

QString GameDatabaseManager::generate_request_roles(MafiaList<Gameplay::Role> roles, FilterType filter)
{
	//Собираем условие для запроса, фильтрующее игры по ролям, которые были
    QString request = "";

    if(roles.length() > 0){
        request = request + " AND (";

		//Тип фильтра точно должен быть указан
        ASSERT((filter != FilterType_NONE), "Roles is not null but filter type is null");

		//Если какая-либо роль участвовала в игре, то строка с зашифрованным списком ролей будет
		//содержать в себе строку с зашифрованной этой одной ролью
        MafiaList<Gameplay::Role> oneRole = MafiaList<Gameplay::Role>() << Gameplay::Role_None;

        for(int i = 0; i < roles.length(); i++){
            oneRole[0] = roles[i];
            QString role = QString::fromStdString(qbytearray_from_qlist<Gameplay::Role>(oneRole).toStdString());
			request += "ROLES LIKE \'%" + role + "%\'";
            if(i != roles.length() - 1){
				//В зависимости от типа фильтра мы можем искать игры, в которых участвовали все роли из списка или хотя бы одна такая
				request += get_sql_filter(filter);
            } else{
                request += ")";
            }
        }
    }

    return request;
}
