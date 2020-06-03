#include "messageprocessingexception.h"

using namespace Mafia;

MessageProcessingException::MessageProcessingException(String data, int id) : MessageParsingException(data, id){}
