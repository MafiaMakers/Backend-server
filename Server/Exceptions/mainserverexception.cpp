#include "mainserverexception.h"
#include "System/functions.h"

using namespace Mafia;
using namespace Exceptions;

MainServerException::MainServerException(){}

MainServerException::MainServerException(System::String data, ExceptionIdType id) : Exception(data, id){}

void MainServerException::show()
{
    std::cout << "Exception in main server manager!!" << "\nException data : " << std::string(data.data, data.size) <<
                 "\nException id : " << id << std::endl;
	close();
}

ExceptionIdType MainServerException::get_base_exception_id()
{
	return MainServerExceptionId_Default;
}

