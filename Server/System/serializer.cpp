#include "serializer.h"
#include "Database/chatmessage.h"
#include "Database/chatsettingsdatabasemanager.h"
#include "userstatistics.h"
#include <MafiaExceptions>
#include "System/tuple.h"
#include "clientinfo.h"


using namespace Mafia;
using namespace System;

template<>
std::string Serializer::serialize(int value){
    std::string result = "";

    for(unsigned int i = 0; i < sizeof(value); i++){
        result += ((char*)&value)[i];
    }
    return result;
}

template<>
int Serializer::deserialize(String data, unsigned int* pointerIndex){
    if((unsigned int)data.size >= sizeof(int)){
        if(pointerIndex != NULL){
            *pointerIndex += sizeof (int);
        }
        return(*(int*)data.data);
    } else{
        throw new Exceptions::SystemException(String("Message is too short"), Exceptions::SystemExceptionId_InvalidMessageSize);
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
Network::Client Serializer::deserialize(String data, unsigned int* pointerIndex){
    Network::Client result = Network::Client();
    unsigned int currentIdx = 0;
    result.ip = deserialize<int>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.port = deserialize<int>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);

    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }

    return result;
}


template<>
std::string Serializer::serialize(Database::ChatCapability value){
    return serialize<int>((int)value);
}

template<>
Database::ChatCapability Serializer::deserialize(String data, unsigned int* pointerIndex){
    return((Database::ChatCapability)deserialize<int>(data, pointerIndex));
}

template<>
std::string Serializer::serialize(Database::ChatFeature value){
    return serialize<int>((int)value);
}

template<>
Database::ChatFeature Serializer::deserialize(String data, unsigned int* pointerIndex){
    return((Database::ChatFeature)deserialize<int>(data, pointerIndex));
}

template<>
std::string Serializer::serialize(Database::AccountType value){
    return serialize<int>((int)value);
}

template<>
Database::AccountType Serializer::deserialize(String data, unsigned int* pointerIndex){
    return((Database::AccountType)deserialize<int>(data, pointerIndex));
}

template<>
std::string Serializer::serialize(Database::Achievement value){
    return serialize<int>((int)value);
}

template<>
Database::Achievement Serializer::deserialize(String data, unsigned int* pointerIndex){
    return((Database::Achievement)deserialize<int>(data, pointerIndex));
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
MafiaList<int> Serializer::deserialize(String data, unsigned int* pointerIndex){
    unsigned int currentIndex = 0;
    int size = deserialize<int>(data, &currentIndex);
    MafiaList<int> result = MafiaList<int>();
    if((unsigned int)data.size >= (size + 1) * sizeof(int)){
        for(int i = 0; i < size; i++){
            result.append(deserialize<int>(String(data.data + currentIndex, sizeof(int)), &currentIndex));
        }
    } else{
        throw new Exceptions::SystemException(String("Message is too short"), Exceptions::SystemExceptionId_InvalidMessageSize);
    }
    if(pointerIndex != NULL){
        *pointerIndex += currentIndex;
    }
    return result;
}


template<>
std::string Serializer::serialize(char value){
    std::string data = "";
    data += value;
    return data;
}

template<>
char Serializer::deserialize(String value, unsigned int* pointerIndex){
    if(value.size > 0){
        if(pointerIndex != NULL){
            *pointerIndex += sizeof (char);
        }
        return value.data[0];
    } else{
        throw new Exceptions::SystemException(String("Message is too short"), Exceptions::SystemExceptionId_InvalidMessageSize);
        return '\0';
    }
}

template<>
std::string Serializer::serialize(bool value){
    return serialize<char>((char)value);
}

template<>
bool Serializer::deserialize(String data, unsigned int* pointerIndex){
    return (bool)deserialize<char>(data, pointerIndex);
}

template<>
std::string Serializer::serialize(MafiaList<char> value){
    std::string data = "";
    data += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        data += serialize(value[i]);
    }
    return data;
}

template<>
MafiaList<char> Serializer::deserialize(String data, unsigned int* pointerIndex){
    unsigned int currentIdx = 0;
    unsigned int size = (unsigned int)deserialize<int>(data, &currentIdx);
    MafiaList<char> result = MafiaList<char>();
    if((unsigned int)data.size >= (size * sizeof(char)) + sizeof(int)){
        for(unsigned int i = 0; i < size; i++){
            result.append(deserialize<char>(String(data.data + currentIdx, sizeof(char)), &currentIdx));
        }
    } else{
        throw new Exceptions::SystemException(String("Message is too short"), Exceptions::SystemExceptionId_InvalidMessageSize);
    }
    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }
    return result;
}

template<>
std::string Serializer::serialize(QString value){
    MafiaList<char> charList = QList<char>();
    std::string data = value.toStdString();

    for(unsigned int i = 0; i < data.length(); i++){
        charList.append(data[i]);
    }

    return serialize<MafiaList<char>>(charList);
}

template<>
QString Serializer::deserialize(String data, unsigned int* pointerIndex){
    QString result = "";
    MafiaList<char> d = deserialize<MafiaList<char>>(data, pointerIndex);
    for(int i = 0; i < d.length(); i++){
        result += d[i];
    }
    return result;
}

template<>
std::string Serializer::serialize(QDateTime value){
    return serialize<QString>(value.toString(Database::SQL_DATETIME_FORMAT));
}

template<>
QDateTime Serializer::deserialize(String data, unsigned int* pointerIndex){
    return QDateTime::fromString(deserialize<QString>(data, pointerIndex), Database::SQL_DATETIME_FORMAT);
}


template<>
std::string Serializer::serialize(Database::ChatMessage value){
    std::string result = "";

    result += serialize<Database::MessageIdType>(value.id);
    result += serialize<Database::UserIdType>(value.from);
    result += serialize<Database::ChatIdType>(value.toChat);
    result += serialize<Database::ChatFeature>(value.feature);
    result += serialize<QDateTime>(value.timestamp);
    result += serialize<QString>(value.data);
    result += serialize<MafiaList<Database::MessageIdType>>(value.answerFor);
    result += serialize<MafiaList<Database::UserIdType>>(value.readUsers);

    return result;
}

template<>
Database::ChatMessage Serializer::deserialize(String data, unsigned int* pointerIndex){
    Database::ChatMessage result = Database::ChatMessage();
    unsigned int currentIdx = 0;

    result.id = deserialize<Database::MessageIdType>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.from = deserialize<Database::UserIdType>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.toChat = deserialize<Database::ChatIdType>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.feature = deserialize<Database::ChatFeature>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.timestamp = deserialize<QDateTime>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.data = deserialize<QString>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.answerFor = deserialize<MafiaList<Database::MessageIdType>>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.readUsers = deserialize<MafiaList<Database::UserIdType>>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);

    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }
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
MafiaList<Database::ChatMessage> Serializer::deserialize(String data, unsigned int* pointerIndex){
    MafiaList<Database::ChatMessage> result = MafiaList<Database::ChatMessage>();
    unsigned int currentIdx = 0;

    unsigned int size = deserialize<int>(data, &currentIdx);
    for(unsigned int i = 0; i < size; i++){
        result.append(deserialize<Database::ChatMessage>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx));
    }
    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }
    return result;
}

template<>
std::string Serializer::serialize(UserStatistics value){
    std::string result = "";
    result += serialize<Database::UserIdType>(value.id);
    result += serialize<QString>(value.nickname);
    result += serialize<Database::AccountType>(value.accountType);
    result += serialize<Database::Achievement>(value.achievement);
    result += serialize<QDateTime>(value.loginDateTime);
    result += serialize<decltype (value.defeatesByRoles)>(value.defeatesByRoles);
    result += serialize<decltype (value.victoriesByRoles)>(value.victoriesByRoles);
    return result;
}

template<>
UserStatistics Serializer::deserialize(String data, unsigned int* pointerIndex){
    UserStatistics result = UserStatistics();
    unsigned int currentIndex = 0;

    result.id = deserialize<Database::UserIdType>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);
    result.nickname = deserialize<QString>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);
    result.accountType = deserialize<Database::AccountType>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);
    result.achievement = deserialize<Database::Achievement>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);
    result.loginDateTime = deserialize<QDateTime>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);
    result.defeatesByRoles = deserialize<MafiaList<int>>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);
    result.victoriesByRoles = deserialize<MafiaList<int>>(String(data.data + currentIndex, data.size - currentIndex), &currentIndex);

    if(pointerIndex != NULL){
        *pointerIndex += currentIndex;
    }
    return result;
}

template<>
std::string Serializer::serialize(System::Tuple<Database::UserIdType, QString> value){
    std::string result = "";
    result += serialize<Database::UserIdType>(value.item1);
    result += serialize<QString>(value.item2);
    return result;
}

template<>
System::Tuple<Database::UserIdType, QString> Serializer::deserialize(String data, unsigned int * pointerIndex){
    System::Tuple<Database::UserIdType, QString> result = System::Tuple<Database::UserIdType, QString>();
    unsigned int currentIdx = 0;
    result.item1 = deserialize<Database::UserIdType>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.item2 = deserialize<QString>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);

    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }

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
ClientInfo Serializer::deserialize(String data, unsigned int* pointerIndex){
    unsigned int currentIdx = 0;
    ClientInfo result = ClientInfo();

    result.client = deserialize<Network::Client>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.statistics = deserialize<UserStatistics>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);

    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }

    return result;
}

template<>
std::string Serializer::serialize(Gameplay::Role value){
    return serialize<int>((int)value);
}

template<>
Gameplay::Role Serializer::deserialize(String data, unsigned int * pointerIndex){
    return (Gameplay::Role)deserialize<int>(data, pointerIndex);
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
MafiaList<Gameplay::Role> Serializer::deserialize(String data, unsigned int * pointerIndex){
    unsigned int currentIdx = 0;
    int size = deserialize<int>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);

    MafiaList<Gameplay::Role> result = MafiaList<Gameplay::Role>();
    for(int i = 0; i < size; i++){
        result.append(deserialize<Gameplay::Role>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx));
    }

    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }

    return result;
}

template<>
std::string Serializer::serialize(Gameplay::GameResult value){
    return serialize<int>((int)value);
}

template<>
Gameplay::GameResult Serializer::deserialize(String data, unsigned int* pointerIndex){
    return (Gameplay::GameResult)deserialize<int>(data, pointerIndex);
}

template<>
std::string Serializer::serialize(Gameplay::Game value){
    std::string result = "";

    result += serialize<Database::GameIdType>(value.id);
    result += serialize<MafiaList<Gameplay::Role>>(value.roles);
    result += serialize<MafiaList<Database::UserIdType>>(value.users);
    result += serialize<Gameplay::GameResult>(value.result);
    result += serialize<QDateTime>(value.endingDT);
    result += serialize<QDateTime>(value.endingDT);

    return result;
}

template<>
Gameplay::Game Serializer::deserialize(String data, unsigned int* pointerIndex){
    Gameplay::Game result = Gameplay::Game();
    unsigned int currentIdx = 0;
    result.id = deserialize<Database::GameIdType>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.roles = deserialize<MafiaList<Gameplay::Role>>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.users = deserialize<MafiaList<Database::UserIdType>>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.result = deserialize<Gameplay::GameResult>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.endingDT = deserialize<QDateTime>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);
    result.beginningDT = deserialize<QDateTime>(String(data.data + currentIdx, data.size - currentIdx), &currentIdx);

    if(pointerIndex != NULL){
        *pointerIndex += currentIdx;
    }

    return result;
}
