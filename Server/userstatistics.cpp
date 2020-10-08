#include "userstatistics.h"
#include "Exceptions/messageparsingexception.h"
#include "QJsonArray"

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

UserStatistics::UserStatistics(QJsonObject src)
{
	QJsonValue valueId = src.value("id");
	QJsonValue valueNickname = src.value("nickname");
	QJsonValue valueAccountType = src.value("accountType");
	QJsonValue valueAchievement = src.value("achievement");
	QJsonValue valueLdt = src.value("loginDateTime");
	QJsonValue valueDbr = src.value("defeatesByRoles");
	QJsonValue valueVbr = src.value("victoriesByRoles");

	if(valueId == QJsonValue::Undefined ||
	   valueNickname == QJsonValue::Undefined ||
	   valueAccountType == QJsonValue::Undefined ||
	   valueAchievement == QJsonValue::Undefined ||
	   valueLdt == QJsonValue::Undefined ||
	   valueDbr == QJsonValue::Undefined ||
	   valueVbr == QJsonValue::Undefined){

		*this = UserStatistics();
		Exceptions::Exception::process_uncatchable_exception(System::String("No needed id in userstatistics message"),
															 Exceptions::MessageParsingExceptionId_NoSuchID);
		return;
	}
	id = valueId.toInt();
	nickname = valueNickname.toString();
	accountType = (Database::AccountType)valueAccountType.toInt();
	achievement = (Database::Achievement)valueAchievement.toInt();
	loginDateTime = QDateTime::fromString(valueLdt.toString(), Database::SQL_DATETIME_FORMAT);

	defeatesByRoles = MafiaList<int>();
	QJsonArray dbrArr = valueDbr.toArray();
	for(int i = 0; i < dbrArr.size(); i++){
		defeatesByRoles.append(dbrArr.at(i).toInt());
	}

	victoriesByRoles = MafiaList<int>();
	QJsonArray vbrArr = valueVbr.toArray();
	for(int i = 0; i < vbrArr.size(); i++){
		victoriesByRoles.append(vbrArr.at(i).toInt());
	}
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

QJsonObject UserStatistics::to_json()
{
	QJsonObject result = QJsonObject();

	result.insert("id", id);
	result.insert("nickname", nickname);
	result.insert("accountType", accountType);
	result.insert("achievement", achievement);
	result.insert("loginDateTime", loginDateTime.toString(Database::SQL_DATETIME_FORMAT));

	QJsonArray dbr = QJsonArray();
	for(int i = 0; i < defeatesByRoles.size(); i++){
		dbr.append(QJsonValue(defeatesByRoles[i]));
	}
	result.insert("defeatesByRoles", dbr);

	QJsonArray vbr = QJsonArray();
	for(int i = 0; i < victoriesByRoles.size(); i++){
		vbr.append(QJsonValue(victoriesByRoles[i]));
	}
	result.insert("victoriesByRoles", vbr);

	return result;
}
