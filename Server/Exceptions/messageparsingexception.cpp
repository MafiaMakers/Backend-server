#include "messageparsingexception.h"
#include <iostream>

using namespace Mafia;

MessageParsingException::MessageParsingException(String data, int id)
{
    this->data = data;
    this->id = defaultId + id;
}

void MessageParsingException::show()
{
    std::cout << "Exception!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

void MessageParsingException::set_data(String data)
{
    this->data = data;
}

void MessageParsingException::set_id(ExceptionIdType id)
{
    this->id = id;
}

ExceptionIdType MessageParsingException::get_id()
{
    return id;
}

String MessageParsingException::get_data()
{
    return data;
}
