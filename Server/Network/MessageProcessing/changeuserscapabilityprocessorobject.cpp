#include "changeuserscapabilityprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

Mafia::Network::MessageProcessing::ChangeUsersCapabilityProcessorObject::ChangeUsersCapabilityProcessorObject(Message message) : ProcessorObject(message){}

void ChangeUsersCapabilityProcessorObject::process()
{
    System::String dataCopy = data;

    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    Database::ChatCapability newCapability = System::Serializer::deserialize<Database::ChatCapability>(dataCopy);

    emit MessageProcessor::instance->change_users_chat_capability(chatId, userId, newCapability, sender);
}
