#include "databaseworkingexception.h"

Mafia::Exceptions::DatabaseWorkingException::DatabaseWorkingException(){}

Mafia::Exceptions::DatabaseWorkingException::DatabaseWorkingException(Mafia::System::String data, ExceptionIdType id) : Exception(data, id){}

void Mafia::Exceptions::DatabaseWorkingException::show()
{
    std::cout << "Exception in work with database!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

ExceptionIdType Mafia::Exceptions::DatabaseWorkingException::get_base_exception_id()
{
    return DatabaseWorkingExceptionId_Default;
}
