#include "clientinfo.h"
using namespace Mafia;

ClientInfo::ClientInfo(QJsonObject src)
{
	QJsonValue valueClient = src.value("client");
	QJsonValue valueStat = src.value("statistics");

	if(valueClient == QJsonValue::Undefined ||
	   valueStat == QJsonValue::Undefined){

		*this = ClientInfo();
		Exceptions::Exception::process_uncatchable_exception(System::String("No needed id in userstatistics message"),
															 Exceptions::MessageParsingExceptionId_NoSuchID);
		return;
	}
	client = Network::Client(valueClient.toObject());
	statistics = UserStatistics(valueStat.toObject());
}

ClientInfo::ClientInfo()
{
	statistics = UserStatistics();
	client = Network::Client();
}

bool ClientInfo::operator == (ClientInfo a)
{
    return (this->statistics.id == a.statistics.id) && (this->client == a.client);
}

bool ClientInfo::operator != (ClientInfo a)
{
	return !(*this == a);
}

QJsonObject ClientInfo::to_json()
{
	QJsonObject result = QJsonObject();
	result.insert("statistics", statistics.to_json());
	result.insert("client", client.to_json());

	return result;
}

