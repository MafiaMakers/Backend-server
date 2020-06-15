#include "messageprocessingexception.h"

using namespace Mafia;
using namespace Exceptions;
MessageProcessingException::MessageProcessingException(System::String data, ExceptionIdType id) : MessageParsingException(data, id){}

ExceptionIdType MessageProcessingException::get_base_exception_id()
{
    return MessageParsingExceptionId_Default;
}
