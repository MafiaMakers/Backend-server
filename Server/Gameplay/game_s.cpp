#include "game_s.h"
#include "Exceptions/gameplayexception.h"
#include "Exceptions/messageparsingexception.h"
#include "QJsonArray"

using namespace Mafia;
using namespace Gameplay;


Mafia::Gameplay::GamePersonalResult Mafia::Gameplay::result_by_role_and_res(Mafia::Gameplay::Role role, Mafia::Gameplay::GameResult res)
{
    switch (res) {
    case GameResult_None:{
        return GamePersonalResult_None;
    }

    case GameResult_MafiaWins:{
        switch (role) {
        case Role_Mafia:{
            return GamePersonalResult_Victory;
        }
        case Role_None:{
            return GamePersonalResult_None;
        }
        default:{
            return GamePersonalResult_Defeat;
        }
        }
        break;
    }
    case GameResult_CiviliansWin:{
        switch(role){
        case Role_Mafia:{
            return GamePersonalResult_Defeat;
        }
        case Role_None:{
            return GamePersonalResult_None;
        }
        default:{
            return GamePersonalResult_Victory;
        }
        }
    }
    default:{
		throw Exceptions::Exception::generate(System::String("Role id not found"), Exceptions::GameplayExceptionId_UnknownRole);
        return GamePersonalResult_None;
    }
    }
}

void Game::show()
{
    std::cout << "-------------------------\nGAME" <<
                 "\nid = " << this->id <<
			 #ifndef DONT_USE_QT
                 "\nstart = " << this->beginningDT.toString(Database::SQL_DATETIME_FORMAT).toStdString() <<
                 "\nend = " << this->endingDT.toString(Database::SQL_DATETIME_FORMAT).toStdString() <<
			 #else
				 "\nstart = " << this->beginningDT <<
				 "\nend = " << this->endingDT <<
			 #endif
                 "\noutcome = " << this->result <<
                 "\nplayers:";
    for(int j = 0; j < this->users.length(); j++){
        std::cout << "\n    id = " << this->users[j] <<
                     "\n    role = " << this->roles[j];
    }
	std::cout << std::endl;
}

bool Game::operator ==(const Game& a) const
{
	if(this->id != a.id){
		return false;
	}
	if(this->roles != a.roles){
		return false;
	}
	if(this->users != a.users){
		return false;
	}
	if(this->result != a.result){
		return false;
	}
	if(!Database::date_time_equals( this->endingDT, a.endingDT )){
		//std::cout << this->endingDT.toString().toStdString() << "\n" << a.endingDT.toString().toStdString() << "\n";
		return false;
	}
	if(!Database::date_time_equals( this->beginningDT, a.beginningDT )){
		//std::cout << this->beginningDT.toString().toStdString() << "\n" << a.beginningDT.toString().toStdString() << "\n";
		return false;
	}
	return true;
}

Game::Game(QJsonObject src)
{
	QJsonValue valueId = src.value("id");
	QJsonValue valueResult = src.value("result");
	QJsonValue valueRoles = src.value("roles");
	QJsonValue valueUsers = src.value("users");
	QJsonValue valueBeginningDT = src.value("beginningDT");
	QJsonValue valueEndingDT = src.value("endingDT");

	if(valueId == QJsonValue::Undefined ||
	   valueRoles == QJsonValue::Undefined ||
	   valueUsers == QJsonValue::Undefined ||
	   valueBeginningDT == QJsonValue::Undefined ||
	   valueEndingDT == QJsonValue::Undefined ||
	   valueResult == QJsonValue::Undefined){

		Exceptions::Exception::process_uncatchable_exception(System::String("No needed id in userstatistics message"),
															 Exceptions::MessageParsingExceptionId_NoSuchID);
		return;
	}

	id = valueId.toInt();
	result = (GameResult)valueResult.toInt();
	beginningDT = QDateTime::fromString(valueBeginningDT.toString(), Database::SQL_DATETIME_FORMAT);
	endingDT = QDateTime::fromString(valueEndingDT.toString(), Database::SQL_DATETIME_FORMAT);

	users = MafiaList<int>();
	QJsonArray usersArr = valueUsers.toArray();
	for(int i = 0; i < usersArr.size(); i++){
		users.append(usersArr.at(i).toInt());
	}

	roles = MafiaList<Role>();
	QJsonArray rolesArr = valueRoles.toArray();
	for(int i = 0; i < rolesArr.size(); i++){
		roles.append((Role)rolesArr.at(i).toInt());
	}

}

Game::Game()
{
	id = -1;
	roles = MafiaList<Role>();
	users = MafiaList<Database::UserIdType>();
	beginningDT = Database::BEGINNING_TIME;
	endingDT = Database::BEGINNING_TIME;
	result = GameResult_None;
}
