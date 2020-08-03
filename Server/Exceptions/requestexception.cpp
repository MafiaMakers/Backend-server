#include "requestexception.h"
#include "System/functions.h"

using namespace Mafia;
using namespace Exceptions;
RequestException::RequestException(System::String data, int id) : Exception(data, id){}

void RequestException::show()
{
	std::cout << "Exception in request!\nException info : " << this->data.data
			  << "\nException id : " << this->id << std::endl;
	close();
}

ExceptionIdType RequestException::get_base_exception_id()
{
	return RequestExceptionId_Defaut;
}
