#include "readchatmessageprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ReadChatMessageProcessorObject::ReadChatMessageProcessorObject(Message message) : ProcessorObject(message){}

void ReadChatMessageProcessorObject::process()
{
    System::String dataCopy = data;

    Database::MessageIdType messageId = System::Serializer::deserialize<Database::MessageIdType>(dataCopy);

    emit MessageProcessor::instance->read_message(sender, messageId);
}
