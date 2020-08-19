#include "keygen.h"
#include "ctime"
using namespace Mafia;
using namespace System;

template<>
std::string KeyGen::generate_key(int length)
{
    std::string result = "";
	srand(time(NULL));
    for(int i = 0; i < length; i++){
		result += (char)('A' + ((unsigned)rand() % 26));
    }
    return result;
}

template<>
String KeyGen::generate_key(int length)
{
	return String(generate_key<std::string>(length));
}

#ifndef DONT_USE_QT
template<>
QString KeyGen::generate_key(int length)
{
	return QString::fromStdString(generate_key<std::string>(length));
}
#endif
