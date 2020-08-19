#include "game_s.h"
#include "Exceptions/gameplayexception.h"
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
