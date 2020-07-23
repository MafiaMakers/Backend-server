#include "chatmessage.h"
#include "databasehelper.h"
using namespace Mafia;
using namespace Database;

void ChatMessage::show()
{
    std::cout << "-------------------------\nMESSAGE" <<
                 "\nid = " << this->id <<
                 "\nfrom = " << this->from <<
                 "\ndata = " << this->data.toStdString() <<
                 "\ndate = " << this->timestamp.toString(SQL_DATETIME_FORMAT).toStdString() <<
                 "\nto chat = " << this->toChat <<
                 "\nfeature = " << this->feature <<
                 "\nreadUsers:";
    for(int j = 0; j < this->readUsers.length(); j++){
        std::cout << "\n    " << this->readUsers[j];
    }
    std::cout << "\nanswer for:";
    for(int j = 0; j < this->answerFor.length(); j++){
        std::cout << "\n    " << this->answerFor[j];
    }
	std::cout << std::endl;
}

bool ChatMessage::operator ==(const ChatMessage& a) const
{
	if(this->id != a.id){
		return false;
	}
	if(this->data != a.data){
		return false;
	}
	if(this->from != a.from){
		return false;
	}
	if(this->toChat != a.toChat){
		return false;
	}
	if(this->feature != a.feature){
		return false;
	}
	if(this->answerFor != a.answerFor){
		return false;
	}
	if(this->readUsers != a.readUsers){
		return false;
	}
	if(this->timestamp.msecsTo(a.timestamp) > 1500 || a.timestamp.msecsTo(this->timestamp) > 1500){
		return false;
	}
	return true;
}
