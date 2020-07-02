#include "addusertochatprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

AddUserToChatProcessorObject::AddUserToChatProcessorObject(Message message) : ProcessorObject(message){}

void AddUserToChatProcessorObject::process()
{
    System::String dataCopy = data;

    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
    Database::ChatCapability newUsersCapability = System::Serializer::deserialize<Database::ChatCapability>(dataCopy);

    emit MessageProcessor::instance->add_user_to_chat(chatId, userId, sender, newUsersCapability);
}
