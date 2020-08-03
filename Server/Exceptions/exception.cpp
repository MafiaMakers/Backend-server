#include "exception.h"
#include "System/logsmanager.h"
#include "System/functions.h"

#include "systemexception.h"
#include "databaseworkingexception.h"
#include "gameplayexception.h"
#include "mainserverexception.h"
#include "messageparsingexception.h"
#include "messageprocessingexception.h"
#include "requestexception.h"
#include "subserverexception.h"

using namespace Mafia;
using namespace Exceptions;

Exception::~Exception()
{

}

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
	close();
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

Exception* Exception::generate(System::String data, ExceptionIdType id)
{
	Exception* exception;

	do{
		if(DatabaseWorkingExceptionId_Default < id && id < DatabaseWorkingExceptionId_Last){
			SAFE_NEW(exception, DatabaseWorkingException(data, id));
			break;
		}
		if(GameplayExceptionId_Default < id && id < GameplayExceptionId_Last){
			SAFE_NEW(exception, GameplayException(data, id));
			break;
		}
		if(MainServerExceptionId_Default < id && id < MainServerExceptionId_Last){
			SAFE_NEW(exception, MainServerException(data, id));
			break;
		}
		if(MessageParsingExceptionId_Default < id && id < MessageParsingExceptionId_Last){
			SAFE_NEW(exception, MessageParsingException(data, id));
			break;
		}
		if(MessageProcessingExceptionId_Default < id && id < MessageProcessingExceptionId_Last){
			SAFE_NEW(exception, MessageProcessingException(data, id));
			break;
		}
		if(RequestExceptionId_Defaut < id && id < RequestExceptionId_Last){
			SAFE_NEW(exception, RequestException(data, id));
			break;
		}
		if(SubserverExceptionId_Default < id && id < SubserverExceptionId_Last){
			SAFE_NEW(exception, SubserverException(data, id));
			break;
		}
		if(SystemExceptionId_Default < id && id < SystemExceptionId_Last){
			SAFE_NEW(exception, SystemException(data, id));
			break;
		}
	} while(false);

	return exception;
}

void Exception::close()
{
	SAFE_DELETE(this);
}
