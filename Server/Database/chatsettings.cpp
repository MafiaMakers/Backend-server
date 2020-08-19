#include "chatsettings.h"
using namespace Mafia;
using namespace Database;

void Chat::show()
{
	std::cout << "-------------------------\nCHAT" <<
				 "\nid = " << this->id <<
			 #ifndef DONT_USE_QT
				 "\ntime = " << this->creationTime.toString(SQL_DATETIME_FORMAT).toStdString() <<
			 #else
				 "\ntime = " << this->creationTime <<
			#endif
				 "\nusers:";
	for(int j = 0; j < this->users.length(); j++){
		std::cout << "\n    id = " << this->users[j] <<
					 "\n    role = " << this->usersCapabilities[j];
	}
	std::cout << std::endl;
}

bool Chat::operator ==(const Chat& a) const
{
	if(this->id != a.id){
		return false;
	}
	if(this->users != a.users){
		return false;
	}
	if(!date_time_equals(this->creationTime, a.creationTime)){
		return false;
	}
	if(this->usersCapabilities != a.usersCapabilities){
		return false;
	}
	return true;
}

bool Chat::operator !=(const Chat& a) const
{
	return !(*this == a);
}
