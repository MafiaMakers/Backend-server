#include "keygen.h"
using namespace Mafia;
using namespace System;

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
	return String(generate_key<std::string>(length));
}

template<>
QString KeyGen::generate_key(int length)
{
	return QString::fromStdString(generate_key<std::string>(length));
}
