#include "readchatmessageprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ReadChatMessageProcessorObject::ReadChatMessageProcessorObject(Message_t message) : ProcessorObject(message){}

void ReadChatMessageProcessorObject::process()
{
	/*System::String dataCopy = data;

	Database::MessageIdType messageId = System::Serializer::deserialize<Database::MessageIdType>(dataCopy);*/

	try {
		GET_FROM_JSON(Database::MessageIdType, messageId, data);
		emit MessageProcessor::instance->read_message(sender, messageId);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
