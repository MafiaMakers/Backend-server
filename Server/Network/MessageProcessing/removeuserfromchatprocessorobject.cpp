#include "removeuserfromchatprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

RemoveUserFromChatProcessorObject::RemoveUserFromChatProcessorObject(Message_t message) : ProcessorObject(message){}

void RemoveUserFromChatProcessorObject::process()
{
	/*System::String dataCopy = data;

    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);

	emit MessageProcessor::instance->remove_user_from_chat(chatId, userId, sender);*/

	try {
		GET_FROM_JSON(Database::UserIdType, userId, data);
		GET_FROM_JSON(Database::ChatIdType, chatId, data);

		emit MessageProcessor::instance->remove_user_from_chat(chatId, userId, sender);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
