#include "deletechatmessageprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

DeleteChatMessageProcessorObject::DeleteChatMessageProcessorObject(Message message) : ProcessorObject(message){}

void DeleteChatMessageProcessorObject::process()
{
    System::String dataCopy = data;

    Database::MessageIdType messageId = System::Serializer::deserialize<Database::MessageIdType>(dataCopy);

    emit MessageProcessor::instance->delete_message(sender, messageId);
}
