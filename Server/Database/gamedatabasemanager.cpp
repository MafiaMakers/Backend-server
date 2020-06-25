#include "gamedatabasemanager.h"
#include "Exceptions/databaseworkingexception.h"
#include "databasehelper.h"
#include "System/functions.h"
using namespace Mafia;
using namespace Database;

GameDatabaseManager::GameDatabaseManager(DatabaseWorker *databaseWorker) : DatabaseManager(databaseWorker, "games_db")
{
    try {
        QString request = "SELECT * FROM " + dbName;
        dbWorker->run_query(request);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            try {
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
        default:{
            exception->show();
            break;
        }
        }
    }
}


#warning "This method is complex to read! Maybe it is good to reconstruct it
Gameplay::Game GameDatabaseManager::get_game_data(GameIdType id)
{
    QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

    try {
        QSqlQuery* query = dbWorker->run_query(request);
        QSqlRecord record = query->record();
        if(query->next()){
            Gameplay::Game game = Gameplay::Game();
            game.id = id;
            game.result = query_value_to_variable<Gameplay::GameResult>(query->value(record.indexOf("GAME_RESULT")));
            game.endingDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("END")));
            game.beginningDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("BEGIN")));
            game.roles = query_value_to_variable<MafiaList<Gameplay::Role>>(query->value(record.indexOf("ROLES")));
            game.users = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("PLAYERS")));

            return game;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
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
    if(game.roles.length() != game.users.length()){
        throw new Exceptions::DatabaseWorkingException(System::String("Users and roles size mismatch"),
                                                       Exceptions::DatabaseWorkingExceptionId_ArraySizeMismatch);
    }

    try {
        QString idRequest = "SELECT COALESCE(MAX(ID), 0) FROM " + dbName;
        QSqlQuery* idQuery = dbWorker->run_query(idRequest);
        if(idQuery->next()){
            game.id = query_value_to_variable<GameIdType>(idQuery->value(0)) + 1;
        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("NULL sql request answer"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }


        QString request = "INSERT INTO " + dbName + " (ID, GAME_RESULT, ROLES, PLAYERS, BEGIN, END) VALUES (%1, %2, %3, %4, %5, %6)";

        request = request.arg(QString::number(game.id));
        request = request.arg(QString::number(game.result));
        request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<Gameplay::Role>(game.roles).toStdString()) + "\'");
        request = request.arg("\'" + QString::fromStdString(qbytearray_from_qlist<UserIdType>(game.users).toStdString()) + "\'");
        request = request.arg("\'" + game.beginningDT.toString(SQL_DATETIME_FORMAT) + "\'");
        request = request.arg("\'" + game.endingDT.toString(SQL_DATETIME_FORMAT) + "\'");

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


#warning "This method is complex to read! Maybe it is good to reconstruct it
MafiaList<Gameplay::Game> GameDatabaseManager::get_games_with(MafiaList<UserIdType> participants, FilterType participantsFilter,
                                                          MafiaList<Gameplay::Role> roles, FilterType rolesFilter,
                                                          MafiaList<Gameplay::GameResult> outcomes, QDateTime beginAfter, QDateTime endBefore)
{
    try {
        QString request = "SELECT * FROM " + dbName + " WHERE (TRUE";

        if(participants.length() > 0){
            request = request + " AND (";
            ASSERT((participantsFilter != FilterType_NONE), "Participants is not null but filter type is null");
            MafiaList<UserIdType> oneUser = MafiaList<UserIdType>() << 0;

            for(int i = 0; i < participants.length(); i++){
                oneUser[0] = participants[i];
                QString user = QString::fromStdString(qbytearray_from_qlist<UserIdType>(oneUser).toStdString());
                request += "CONTAINS(PLAYERS, \'" + user + "\')";

                if(i != participants.length() - 1){
                    switch (participantsFilter) {
                    case FilterType_OR:{
                        request += " OR ";
                        break;
                    }
                    case FilterType_AND:{
                        request += " AND ";
                        break;
                    }
                    default:{
                        throw new Exceptions::DatabaseWorkingException(System::String("Unknown filtertype"), Exceptions::DatabaseWorkingExceptionId_UnknownFilterType);
                    }
                    }
                } else{
                    request += ")";
                }
            }

        }

        if(outcomes.length() > 0){
            request = request + " AND (";

            for(int i = 0; i < outcomes.length(); i++){
                QString outcome = QString::number(outcomes[i]);
                request += "(GAME_RESULT = " + outcome + ")";
                if(i != roles.length() - 1){
                    request += " OR ";
                } else{
                    request += ")";
                }
            }
        }

        if(roles.length() > 0){
            request = request + " AND (";
            ASSERT((rolesFilter != FilterType_NONE), "Roles is not null but filter type is null");
            MafiaList<Gameplay::Role> oneRole = MafiaList<Gameplay::Role>() << Gameplay::Role_None;

            for(int i = 0; i < roles.length(); i++){
                oneRole[0] = roles[i];
                QString role = QString::fromStdString(qbytearray_from_qlist<Gameplay::Role>(oneRole).toStdString());
                request += "CONTAINS(ROLES, \'" + role + "\')";
                if(i != roles.length() - 1){
                    switch (rolesFilter) {
                    case FilterType_OR:{
                        request += " OR ";
                        break;
                    }
                    case FilterType_AND:{
                        request += " AND ";
                        break;
                    }
                    default:{
                        throw new Exceptions::DatabaseWorkingException(System::String("Unknown filtertype"), Exceptions::DatabaseWorkingExceptionId_UnknownFilterType);
                    }
                    }
                } else{
                    request += ")";
                }
            }
        }

        request = request + " AND (BEGIN >= \'" + beginAfter.toString(SQL_DATETIME_FORMAT) + "\')";
        request = request + " AND (END <= \'" + endBefore.toString(SQL_DATETIME_FORMAT) + "\')";
        request += ")";

        QSqlQuery* query = dbWorker->run_query(request);
        QSqlRecord record = query->record();
        MafiaList<Gameplay::Game> games = MafiaList<Gameplay::Game>();
        while(query->next()){
            Gameplay::Game current = Gameplay::Game();

            current.id = query->value(record.indexOf("ID")).toInt();
            current.result = query_value_to_variable<Gameplay::GameResult>(query->value(record.indexOf("GAME_RESULT")));
            current.beginningDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("BEGIN")));
            current.endingDT = query_value_to_variable<QDateTime>(query->value(record.indexOf("END")));
            current.roles = query_value_to_variable<MafiaList<Gameplay::Role>>(query->value(record.indexOf("ROLES")));
            current.users = query_value_to_variable<MafiaList<UserIdType>>(query->value(record.indexOf("PLAYERS")));
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
