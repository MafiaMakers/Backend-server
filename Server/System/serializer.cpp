#include "serializer.h"
#include <MafiaDatabase>
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
std::string Serializer::serialize(Database::ChatCapability value){
    return serialize<int>((int)value);
}

template<>
std::string Serializer::serialize(Database::ChatFeature value){
    return serialize<int>((int)value);
}

template<>
std::string Serializer::serialize(MafiaList<int> value){
    std::string result = "";//String(new char[sizeof(int) * (value.length() + 1)]);

    result += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        result += serialize<int>(value[i]);
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
std::string Serializer::serialize(MafiaList<char> value){
    std::string data = "";
    data += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        data += serialize(value[i]);
    }
    return data;
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
std::string Serializer::serialize(QDateTime value){
    return serialize<QString>(value.toString(Database::SQL_DATETIME_FORMAT));
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
std::string Serializer::serialize(MafiaList<Database::ChatMessage> value){
    std::string result ="";
    result += serialize<int>(value.length());
    for(int i = 0; i < value.length(); i++){
        result += serialize<Database::ChatMessage>(value[i]);
    }
    return result;
}
