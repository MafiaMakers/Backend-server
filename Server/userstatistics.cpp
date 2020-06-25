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
