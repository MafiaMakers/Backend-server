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
