#include "getchatmessagesprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

GetChatMessagesProcessorObject::GetChatMessagesProcessorObject(Message message) : ProcessorObject(message){}

void GetChatMessagesProcessorObject::process()
{
    System::String dataCopy = data;

    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    int messagesCount = System::Serializer::deserialize<int>(dataCopy);

    emit MessageProcessor::instance->get_last_messages(sender, chatId, id, messagesCount);
}
