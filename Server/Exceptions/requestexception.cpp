#include "requestexception.h"
using namespace Mafia;

RequestException::RequestException(String data, int id) : Mafia::Exception(data, id){}

void RequestException::show()
{
    std::cout << "Exception in request!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

ExceptionIdType RequestException::get_base_exception_id()
{
    return REQUEST_EXCEPTION_DEFAULT_EXCEPTION_ID;
}


