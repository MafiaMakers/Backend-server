#include "messageprocessingexception.h"

using namespace Mafia;

MessageProcessingException::MessageProcessingException(String data, int id) : MessageParsingException(data, id){}

ExceptionIdType MessageProcessingException::get_base_exception_id()
{
    return MESSAGE_PROCESSING_EXCEPTION_DEFAULT_EXCEPTION_ID;
}
