#include "requestexception.h"
using namespace Mafia;

RequestException::RequestException(String data, ExceptionIdType id)
{
    this->data = data;
    this->id = id + RequestException::defaultId;
}

void RequestException::show()
{
    std::cout << "Exception in request!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

void RequestException::set_id(ExceptionIdType id)
{
    this->id = id + RequestException::defaultId;
}

void RequestException::set_data(String data)
{
    this->data = data;
}

ExceptionIdType RequestException::get_id()
{
    return id;
}

String RequestException::get_data()
{
    return data;
}


