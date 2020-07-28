#include "exception.h"
#include "System/logsmanager.h"

using namespace Mafia;
using namespace Exceptions;

Exception::Exception(System::String data, ExceptionIdType id)
{
    this->data = data;
    this->id = id;

	System::LogsManager::add_record(std::string(data.data, data.size),
									System::LogType_Exception,
									System::LogSource_MainServer,
									"");
}

void Exception::show()
{
    std::cout << "Exception!\nException info : " << this->data.data << "\nException id : " << this->id << std::endl;
}

void Exception::set_data(System::String data)
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

System::String Exception::get_data()
{
    return data;
}
