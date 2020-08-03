#include "subserverexception.h"
#include "System/functions.h"

using namespace Mafia;
using namespace Exceptions;
SubserverException::SubserverException(System::String data, int id) : Exception(data, id){}

void SubserverException::show(){
	std::cout << "Exception in subserver!\nException info : " << this->data.data
			  << "\nException id : " << this->id << std::endl;
	close();
}

ExceptionIdType SubserverException::get_base_exception_id()
{
	return SubserverExceptionId_Default;
}
