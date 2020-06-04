#include "subserverexception.h"
using namespace Mafia;

SubserverException::SubserverException(String data, int id) : Mafia::Exception(data, id){}

void SubserverException::show(){
    std::cout << "Exception in subserver!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

ExceptionIdType SubserverException::get_base_exception_id()
{
    return SUBSERVER_EXCEPTION_DEFAULT_EXCEPTION_ID;
}
