#include "systemexception.h"
using namespace Mafia;
using namespace Exceptions;

SystemException::SystemException(){}

SystemException::SystemException(System::String data, ExceptionIdType id) : Exception(data, id){}

void SystemException::show()
{
    std::cout << "Exception in system function!\nException data : " << std::string(data.data, data.size) << "\nException id : " << id << std::endl;
}

ExceptionIdType SystemException::get_base_exception_id()
{
    return SystemExceptionId_Default;
}
