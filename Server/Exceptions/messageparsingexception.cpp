#include "messageparsingexception.h"
#include <iostream>

using namespace Mafia;

MessageParsingException::MessageParsingException(String data, ExceptionIdType id) : Mafia::Exception(data, id){}

void MessageParsingException::show()
{
    std::cout << "Exception in parsing message!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

ExceptionIdType MessageParsingException::get_base_exception_id()
{
    return MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID;
}
