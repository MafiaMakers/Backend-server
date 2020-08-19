#include "serializer.h"
#include "Database/chatmessage.h"
#include "Database/chatsettings.h"
#include "userstatistics.h"
#include <MafiaExceptions>
#include "System/tuple.h"
#include "clientinfo.h"
#include "Network/message.h"
#include "Gameplay/game_s.h"

#ifndef DONT_USE_QT
	#include "Database/databasehelper.h"
#endif

using namespace Mafia;
using namespace System;

template<>
std::string Serializer::serialize(char value){
    std::string data = "";
	data += 'M' + (char)(value / 16);
	data += 'M' + (char)(value % 16);
    return data;
}

template<>
char Serializer::deserialize(String &value){
	if(value.size >= 2){
		char result = (value.data[0] - 'M') * 16 + (value.data[1] - 'M');

		/*Здесь должна быть работа с памятью, но ее не будет
		 *т.к. возникает мильон ошибок повторного разыменовывания указателей
		 *и с ними я совсем сейчас не хочу разбираться.
		 *Так что если возникнет проблема утечек памяти, то будем потом проверять эти места
		 *А пока просто забьем*/

		//delete value.data;
		//delete (value.data + 1);
		value.data += 2;
		value.size -= 2;
		//value = String(value.data + 2 * sizeof(char), value.size - 2 * sizeof(char));
        return result;
    } else{
		throw Exceptions::Exception::generate(String("Message is too short"), Exceptions::SystemExceptionId_InvalidMessageSize);
        return '\0';
    }
}

template<>
std::string Serializer::serialize(int value){
	std::string result = "";

	for(unsigned int i = 0; i < sizeof(value); i++){
		result += serialize<char>((((char*)&value)[i]));
	}
	return result;
}

template<>
int Serializer::deserialize(String &data){
	if((unsigned int)data.size >= sizeof(int)){
		char* d;
		SAFE_NEW(d, char[sizeof (int)]);
		for(unsigned int i = 0; i < sizeof(int); i++){
			d[i] = deserialize<char>(data);
		}
		int result = *(int*)d;
		SAFE_DELETE_ARRAY(d);
		return result;
	} else{
		throw Exceptions::Exception::generate(String("Message is too short"),
											  Exceptions::SystemExceptionId_InvalidMessageSize);
		return 0;
	}
}


template<>
std::string Serializer::serialize(Network::Client value){
	std::string result = "";
	result += serialize<int>(value.ip);
	result += serialize<int>(value.port);

	return result;
}

template<>
Network::Client Serializer::deserialize(String &data){
	Network::Client result = Network::Client();
	result.ip = deserialize<int>(data);
	result.port = deserialize<int>(data);
	return result;
}


template<>
std::string Serializer::serialize(Database::ChatCapability value){
	return serialize<int>((int)value);
}

template<>
Database::ChatCapability Serializer::deserialize(String &data){
	return((Database::ChatCapability)deserialize<int>(data));
}

template<>
std::string Serializer::serialize(Database::ChatFeature value){
	return serialize<int>((int)value);
}

template<>
Database::ChatFeature Serializer::deserialize(String &data){
	return((Database::ChatFeature)deserialize<int>(data));
}

template<>
std::string Serializer::serialize(Database::AccountType value){
	return serialize<int>((int)value);
}

template<>
Database::AccountType Serializer::deserialize(String &data){
	return((Database::AccountType)deserialize<int>(data));
}

template<>
std::string Serializer::serialize(Database::Achievement value){
	return serialize<int>((int)value);
}

template<>
Database::Achievement Serializer::deserialize(String &data){
	return((Database::Achievement)deserialize<int>(data));
}

template<>
std::string Serializer::serialize(MafiaList<int> value){
	std::string result = "";

	result += serialize<int>(value.length());
	for(int i = 0; i < value.length(); i++){
		result += serialize<int>(value[i]);
	}

	return result;
}

template<>
MafiaList<int> Serializer::deserialize(String &data){

	int size = deserialize<int>(data);
	MafiaList<int> result = MafiaList<int>();
	if((unsigned int)data.size >= size * sizeof(int)){
		for(int i = 0; i < size; i++){
			result.append(deserialize<int>(data));
		}
	} else{
		throw Exceptions::Exception::generate(String("Message is too short"), Exceptions::SystemExceptionId_InvalidMessageSize);
	}

	return result;
}

template<>
std::string Serializer::serialize(bool value){
    return serialize<char>((char)value);
}

template<>
bool Serializer::deserialize(String &data){
    return (bool)deserialize<char>(data);
}

template<>
std::string Serializer::serialize(MafiaList<char> value){
    std::string data = "";
    data += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        data += serialize<char>(value[i]);
    }
    return data;
}

template<>
MafiaList<char> Serializer::deserialize(String &data){

    unsigned int size = (unsigned int)deserialize<int>(data);
    MafiaList<char> result = MafiaList<char>();
	for(unsigned int i = 0; i < size; i++){
		result.append(deserialize<char>(data));
	}

    return result;
}

template<>
std::string Serializer::serialize(STRING value){
	MafiaList<char> charList = MafiaList<char>();
#ifndef DONT_USE_QT
    std::string data = value.toStdString();
#else
	std::string data = value;
#endif
    for(unsigned int i = 0; i < data.length(); i++){
        charList.append(data[i]);
    }

    return serialize<MafiaList<char>>(charList);
}

template<>
STRING Serializer::deserialize(String &data){
	STRING result = "";
    MafiaList<char> d = deserialize<MafiaList<char>>(data);
    for(int i = 0; i < d.length(); i++){
        result += d[i];
    }
    return result;
}

#ifndef DONT_USE_QT
template<>
std::string Serializer::serialize(QDateTime value){
    return serialize<QString>(value.toString(Database::SQL_DATETIME_FORMAT));
}

template<>
QDateTime Serializer::deserialize(String &data){
	return Database::from_string(deserialize<QString>(data));
}
#endif

template<>
std::string Serializer::serialize(Database::ChatMessage value){
    std::string result = "";

    result += serialize<Database::MessageIdType>(value.id);
    result += serialize<Database::UserIdType>(value.from);
    result += serialize<Database::ChatIdType>(value.toChat);
    result += serialize<Database::ChatFeature>(value.feature);
	result += serialize<DATE_TIME>(value.timestamp);
	result += serialize<STRING>(value.data);
    result += serialize<MafiaList<Database::MessageIdType>>(value.answerFor);
    result += serialize<MafiaList<Database::UserIdType>>(value.readUsers);

    return result;
}

template<>
Database::ChatMessage Serializer::deserialize(String &data){
    Database::ChatMessage result = Database::ChatMessage();


    result.id = deserialize<Database::MessageIdType>(data);
    result.from = deserialize<Database::UserIdType>(data);
    result.toChat = deserialize<Database::ChatIdType>(data);
    result.feature = deserialize<Database::ChatFeature>(data);
	result.timestamp = deserialize<DATE_TIME>(data);
	result.data = deserialize<STRING>(data);
    result.answerFor = deserialize<MafiaList<Database::MessageIdType>>(data);
    result.readUsers = deserialize<MafiaList<Database::UserIdType>>(data);

    return result;
}

template<>
std::string Serializer::serialize(MafiaList<Database::ChatMessage> value){
    std::string result ="";
    result += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        result += serialize<Database::ChatMessage>(value[i]);
    }
    return result;
}

template<>
MafiaList<Database::ChatMessage> Serializer::deserialize(String &data){
    MafiaList<Database::ChatMessage> result = MafiaList<Database::ChatMessage>();


    unsigned int size = deserialize<int>(data);
    for(unsigned int i = 0; i < size; i++){
        result.append(deserialize<Database::ChatMessage>(data));
    }

    return result;
}

template<>
std::string Serializer::serialize(UserStatistics value){
    std::string result = "";
    result += serialize<Database::UserIdType>(value.id);
	result += serialize<STRING>(value.nickname);
    result += serialize<Database::AccountType>(value.accountType);
    result += serialize<Database::Achievement>(value.achievement);
	result += serialize<DATE_TIME>(value.loginDateTime);
    result += serialize<decltype (value.defeatesByRoles)>(value.defeatesByRoles);
    result += serialize<decltype (value.victoriesByRoles)>(value.victoriesByRoles);
    return result;
}

template<>
UserStatistics Serializer::deserialize(String &data){
    UserStatistics result = UserStatistics();


    result.id = deserialize<Database::UserIdType>(data);
	result.nickname = deserialize<STRING>(data);
    result.accountType = deserialize<Database::AccountType>(data);
    result.achievement = deserialize<Database::Achievement>(data);
	result.loginDateTime = deserialize<DATE_TIME>(data);
    result.defeatesByRoles = deserialize<MafiaList<int>>(data);
    result.victoriesByRoles = deserialize<MafiaList<int>>(data);
    return result;
}

template<>
std::string Serializer::serialize(System::Tuple<Database::UserIdType, STRING> value){
    std::string result = "";
    result += serialize<Database::UserIdType>(value.item1);
	result += serialize<STRING>(value.item2);
    return result;
}

template<>
System::Tuple<Database::UserIdType, STRING> Serializer::deserialize(String &data){
	System::Tuple<Database::UserIdType, STRING> result = System::Tuple<Database::UserIdType, STRING>();

    result.item1 = deserialize<Database::UserIdType>(data);
	result.item2 = deserialize<STRING>(data);

    return result;
}

template<>
std::string Serializer::serialize(ClientInfo value){
    std::string result = "";

    result += serialize<Network::Client>(value.client);
    result += serialize<UserStatistics>(value.statistics);

    return result;
}

template<>
ClientInfo Serializer::deserialize(String &data){

    ClientInfo result = ClientInfo();

    result.client = deserialize<Network::Client>(data);
    result.statistics = deserialize<UserStatistics>(data);
    return result;
}

template<>
std::string Serializer::serialize(Gameplay::Role value){
    return serialize<int>((int)value);
}

template<>
Gameplay::Role Serializer::deserialize(String &data){
    return (Gameplay::Role)deserialize<int>(data);
}

template<>
std::string Serializer::serialize(MafiaList<Gameplay::Role> value){
    std::string result = "";
    result += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        result += serialize<Gameplay::Role>(value[i]);
    }

    return result;
}

template<>
MafiaList<Gameplay::Role> Serializer::deserialize(String &data){

    int size = deserialize<int>(data);

    MafiaList<Gameplay::Role> result = MafiaList<Gameplay::Role>();
    for(int i = 0; i < size; i++){
        result.append(deserialize<Gameplay::Role>(data));
    }
    return result;
}

template<>
std::string Serializer::serialize(Gameplay::GameResult value){
    return serialize<int>((int)value);
}

template<>
Gameplay::GameResult Serializer::deserialize(String &data){
    return (Gameplay::GameResult)deserialize<int>(data);
}

template<>
std::string Serializer::serialize(Gameplay::Game value){
    std::string result = "";

    result += serialize<Database::GameIdType>(value.id);
    result += serialize<MafiaList<Gameplay::Role>>(value.roles);
    result += serialize<MafiaList<Database::UserIdType>>(value.users);
    result += serialize<Gameplay::GameResult>(value.result);
	result += serialize<DATE_TIME>(value.endingDT);
	result += serialize<DATE_TIME>(value.beginningDT);

    return result;
}

template<>
Gameplay::Game Serializer::deserialize(String &data){
    Gameplay::Game result = Gameplay::Game();

    result.id = deserialize<Database::GameIdType>(data);
    result.roles = deserialize<MafiaList<Gameplay::Role>>(data);
    result.users = deserialize<MafiaList<Database::UserIdType>>(data);
    result.result = deserialize<Gameplay::GameResult>(data);
	result.endingDT = deserialize<DATE_TIME>(data);
	result.beginningDT = deserialize<DATE_TIME>(data);

    return result;
}

template<>
std::string Serializer::serialize(System::Tuple<int, int> value){
    std::string result = "";
    result += serialize<int>(value.item1);
    result += serialize<int>(value.item2);

    return result;
}

template<>
System::Tuple<int, int> Serializer::deserialize(System::String &data){
    System::Tuple<int, int> result = System::Tuple<int, int>();

    result.item1 = deserialize<int>(data);
    result.item2 = deserialize<int>(data);

    return result;
}

template<>
std::string Serializer::serialize(Network::MessageType value){
    return serialize<int>((int)value);
}

template<>
Network::MessageType Serializer::deserialize(System::String &data){
    return (Network::MessageType)deserialize<int>(data);
}

template<>
std::string Serializer::serialize(Network::Message value){
    std::string result = "";

    result += serialize<Network::MessageIdType>(value.id);
    result += serialize<Network::MessageType>(value.type);
    result += serialize<int>(value.partIndex);
    result += serialize<int>(value.partsCount);
    result += serialize<Network::Client>(value.client);
#ifndef DONT_USE_QT
	result += serialize<STRING>(QString::fromStdString(std::string((char*)value.data, value.size)));
#else
	result += serialize<STRING>(std::string((char*)value.data, value.size));
#endif
    return result;
}

template<>
Network::Message System::Serializer::deserialize(String &data){
    Network::Message result = Network::Message();
    result.id = deserialize<Network::MessageIdType>(data);
    result.type = deserialize<Network::MessageType>(data);
    result.partIndex = deserialize<int>(data);
    result.partsCount = deserialize<int>(data);
    result.client = deserialize<Network::Client>(data);
#ifndef DONT_USE_QT
	std::string mesDataStr = deserialize<STRING>(data).toStdString();
#else
	std::string mesDataStr = deserialize<STRING>(data);
#endif
	String mesData = String(mesDataStr);
	result.data = (Network::SymbolType*)mesData.data;
	result.size = mesData.size;

    return result;
}

template<>
std::string Serializer::serialize(Tuple<Network::Message, int> value){
    std::string result = "";
    result += serialize<Network::Message>(value.item1);
    result += serialize<int>(value.item2);
    return result;
}

template<>
Tuple<Network::Message, int> Serializer::deserialize(System::String &data){
    Tuple<Network::Message, int> result = Tuple<Network::Message, int>();

    result.item1 = deserialize<Network::Message>(data);
    result.item2 = deserialize<int>(data);
    return result;
}
