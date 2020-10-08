#include "getchatmessagesprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

GetChatMessagesProcessorObject::GetChatMessagesProcessorObject(Message_t message) : ProcessorObject(message){}

void GetChatMessagesProcessorObject::process()
{
	/*System::String dataCopy = data;

    Database::ChatIdType chatId = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    int messagesCount = System::Serializer::deserialize<int>(dataCopy);

	emit MessageProcessor::instance->get_last_messages(sender, chatId, id, messagesCount);*/

	try {
		GET_FROM_JSON(int, messagesCount, data);
		GET_FROM_JSON(Database::ChatIdType, chatId, data);

		emit MessageProcessor::instance->get_last_messages(sender, chatId, id, messagesCount);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
