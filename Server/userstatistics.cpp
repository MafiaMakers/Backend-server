#include "userstatistics.h"

using namespace Mafia;



UserStatistics::UserStatistics(){}

UserStatistics::UserStatistics(Database::User user)
{
    this->id = user.id;
    this->nickname = user.nickname;
    this->accountType = user.accountType;
    this->achievement = user.achievement;
    this->loginDateTime = user.loginDateTime;
    this->defeatesByRoles = user.defeatesByRoles;
	this->victoriesByRoles = user.victoriesByRoles;
}

UserStatistics::~UserStatistics()
{

}

bool UserStatistics::operator ==(const UserStatistics& a) const
{
	if(this->id != a.id){
		return false;
	}
	if(this->nickname != a.nickname){
		return false;
	}
	if(this->accountType != a.accountType){
		return false;
	}
	if(this->achievement != a.achievement){
		return false;
	}
	if(!Database::date_time_equals(this->loginDateTime, a.loginDateTime)){
		return false;
	}
	if(this->defeatesByRoles != a.defeatesByRoles){
		return false;
	}
	if(this->victoriesByRoles != a.victoriesByRoles){
		return false;
	}
	return true;
}
