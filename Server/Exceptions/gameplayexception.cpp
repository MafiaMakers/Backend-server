#include "gameplayexception.h"
#include "System/functions.h"
using namespace Mafia;
using namespace Exceptions;


GameplayException::GameplayException(){}

Mafia::Exceptions::GameplayException::GameplayException(Mafia::System::String data, ExceptionIdType id) : Exception(data, id){}

void Mafia::Exceptions::GameplayException::show()
{
    std::cout << "Exception in gameplay!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
	close();
}

ExceptionIdType Mafia::Exceptions::GameplayException::get_base_exception_id()
{
	return Mafia::Exceptions::GameplayExceptionId_Default;
}
