#include "messageparsingexception.h"
#include "System/functions.h"
#include <iostream>

using namespace Mafia;
using namespace Exceptions;
MessageParsingException::MessageParsingException(System::String data, ExceptionIdType id) : Exception(data, id){}

void MessageParsingException::show()
{
	std::cout << "Exception in parsing message!\nException info : " << this->data.data
			  << "\nException id : " << this->id << std::endl;
	close();
}

ExceptionIdType MessageParsingException::get_base_exception_id()
{
	return MessageParsingExceptionId_Default;
}
