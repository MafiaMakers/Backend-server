#include "user.h"
#include "databasehelper.h"
using namespace Mafia;
using namespace Database;

void User::show()
{
    std::cout << "--------------\nUSER" <<
                 "\nid = " << this->id <<
                 "\nsalt = " << this->salt.toStdString() <<
                 "\nhash = " << this->passwordHash.toStdString() <<
                 "\nemail = " << this->email.toStdString() <<
                 "\nnickname = " << this->nickname.toStdString() <<
                 "\ndate = " << this->loginDateTime.toString(SQL_DATETIME_FORMAT).toStdString() <<
                 "\nauthorized = " << this->authorized <<
                 "\nstatus = " << this->isConfirmed <<
                 "\naccount type = " << this->accountType <<
                 "\nachievement = " << this->achievement <<
                 "\nconfirmation key = " << this->confirmationKey.toStdString() <<
                 "\nchats:";
    for(int j = 0; j < this->chats.length(); j++){
        std::cout << "\n    " << this->chats[j];
    }
    std::cout << "\ngames:";
    for(int j = 0; j < this->allGames.length(); j++){
        std::cout << "\n    " << this->allGames[j];
    }
    std::cout << "\nvictories:";
    for(int j = 0; j < this->victoriesByRoles.length(); j++){
        std::cout << "\n    " << this->victoriesByRoles[j];
    }
    std::cout << "\ndefeates:";
    for(int j = 0; j < this->defeatesByRoles.length(); j++){
        std::cout << "\n    " << this->defeatesByRoles[j];
    }
    std::cout << "\ntransactions:";
    for(int j = 0; j < this->transactions.length(); j++){
        std::cout << "\n    " << this->transactions[j];
    }
	std::cout << std::endl;
}

bool User::operator ==(const User& user) const
{
	if(user.id != this->id){
		return false;
	}
	if(user.salt != this->salt){
		return false;
	}
	if(user.chats != this->chats){
		return false;
	}
	if(user.email != this->email){
		return false;
	}
	if(user.allGames != this->allGames){
		return false;
	}
	if(user.nickname != this->nickname){
		return false;
	}
	if(user.authorized != this->authorized){
		return false;
	}
	if(user.accountType != this->accountType){
		return false;
	}
	if(user.achievement != this->achievement){
		return false;
	}
	if(user.isConfirmed != this->isConfirmed){
		return false;
	}
	if(user.passwordHash != this->passwordHash){
		return false;
	}
	if(user.transactions != this->transactions){
		return false;
	}
	if(user.loginDateTime != this->loginDateTime){
		return false;
	}
	if(user.confirmationKey != this->confirmationKey){
		return false;
	}
	if(user.defeatesByRoles != this->defeatesByRoles){
		return false;
	}
	if(user.victoriesByRoles != this->victoriesByRoles){
		return false;
	}
	return true;
}

bool User::operator !=(const User& user) const
{
	return !(*this == user);
}

void Transaction::show()
{
    std::cout << "-----------------------\nTRANSACTION" <<
                 "\nid = " << this->id <<
                 "\nuser = " << this->buyer <<
                 "\nhash = " << this->hash.toStdString() <<
                 "\ntype = " << this->type <<
                 "\ndate = " << this->timestamp.toString(SQL_DATETIME_FORMAT).toStdString() <<
				 "\nprice = " << this->price << "\n";
}

bool Transaction::operator ==(const Transaction& a) const
{
	if(this->id != a.id){
		return false;
	}
	if(this->hash != a.hash){
		return false;
	}
	if(this->buyer != a.buyer){
		return false;
	}
	if(this->type != a.type){
		return false;
	}
	if(this->price != a.price){
		return false;
	}
	if(this->timestamp != a.timestamp){
		return false;
	}
	return true;
}
