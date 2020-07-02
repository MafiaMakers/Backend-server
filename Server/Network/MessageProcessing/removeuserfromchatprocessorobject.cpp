#include "removeuserfromchatprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

RemoveUserFromChatProcessorObject::RemoveUserFromChatProcessorObject(Message message) : ProcessorObject(message){}

void RemoveUserFromChatProcessorObject::process()
{
    System::String dataCopy = data;

    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);

    emit MessageProcessor::instance->remove_user_from_chat(chatId, userId, sender);
}
