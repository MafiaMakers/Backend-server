#include "addusertochatprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

AddUserToChatProcessorObject::AddUserToChatProcessorObject(Message_t message) : ProcessorObject(message){}

void AddUserToChatProcessorObject::process()
{
	/*System::String dataCopy = data;

    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
	Database::ChatCapability newUsersCapability = System::Serializer::deserialize<Database::ChatCapability>(dataCopy);*/
	try {
		GET_FROM_JSON(Database::UserIdType, userId, data);
		GET_FROM_JSON(Database::ChatIdType, chatId, data);
		GET_FROM_JSON(Database::ChatCapability, newUsersCapability, data);

		emit MessageProcessor::instance->add_user_to_chat(chatId, userId, sender, newUsersCapability);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}

}
