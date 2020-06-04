#include "exception.h"

using namespace Mafia;

Exception::Exception(String data, ExceptionIdType id)
{
    this->data = data;
    this->id = id;
}

void Exception::show()
{
    std::cout << "Exception!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

void Exception::set_data(String data)
{
    this->data = data;
}

void Exception::set_id(ExceptionIdType id)
{
    this->id = id;
}

ExceptionIdType Exception::get_id()
{
    return id;
}

String Exception::get_data()
{
    return data;
}
