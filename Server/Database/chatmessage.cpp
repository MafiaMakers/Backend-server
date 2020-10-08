#include "chatmessage.h"
#include "QJsonArray"
//#include "databasehelper.h"
using namespace Mafia;
using namespace Database;

void ChatMessage::show()
{
    std::cout << "-------------------------\nMESSAGE" <<
                 "\nid = " << this->id <<
                 "\nfrom = " << this->from <<
				#ifndef DONT_USE_QT
				"\ndata = " << this->data.toStdString() <<
				 "\ndate = " << this->timestamp.toString(SQL_DATETIME_FORMAT).toStdString() <<
				#else
				"\ndata = " << this->data <<
				 "\ntime = " << this->timestamp <<
				#endif
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
	if(!date_time_equals(this->timestamp, a.timestamp)){
		return false;
	}
	return true;
}

QJsonObject ChatMessage::to_json()
{
	QJsonObject result = QJsonObject();

	result.insert("id", id);
	result.insert("data", data);
	result.insert("from", from);
	result.insert("toChat", toChat);
	result.insert("feature", feature);
	result.insert("timestamp", timestamp.toString(Database::SQL_DATETIME_FORMAT));

	QJsonArray answerForArr = QJsonArray();
	for(int i = 0; i < answerFor.size(); i++){
		answerForArr.append(QJsonValue(answerFor[i]));
	}
	result.insert("answerFor", answerForArr);

	QJsonArray readUsersArr = QJsonArray();
	for(int i = 0; i < readUsers.size(); i++){
		readUsersArr.append(QJsonValue(readUsers[i]));
	}
	result.insert("readUsers", readUsersArr);

	return result;
}

ChatMessage::ChatMessage()
{
	id = -1;
	data = "";
	from = -1;
	toChat = -1;
	feature = ChatFeature_None;
	timestamp = Database::BEGINNING_TIME;
}
