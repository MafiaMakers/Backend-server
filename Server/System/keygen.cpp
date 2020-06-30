#include "keygen.h"
using namespace Mafia;
using namespace System;

template<>
QString KeyGen::generate_key(int length)
{
    QString result = "";

    for(int i = 0; i < length; i++){
        result += (char)('A' + (qrand() % 26));
    }
    return result;
}

template<>
std::string KeyGen::generate_key(int length)
{
    std::string result = "";

    for(int i = 0; i < length; i++){
        result += (char)('A' + (qrand() % 26));
    }
    return result;
}

template<>
String KeyGen::generate_key(int length)
{
    String result = String(new char[length+1], length);
    result.data[length] = '\0';

    for(int i = 0; i < length; i++){
        result.data[i] = (char)('A' + (qrand() % 26));
    }
    return result;
}
