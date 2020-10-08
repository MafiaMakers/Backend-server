#include "sendchatmessageprocessorobject.h"
#include "System/serializer.h"
using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

SendChatMessageProcessorObject::SendChatMessageProcessorObject(Message_t message) : ProcessorObject(message){}

void SendChatMessageProcessorObject::process()
{
	//System::String dataCopy = data;

	/*Database::ChatIdType toChat = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    QString messageData = System::Serializer::deserialize<QString>(dataCopy);
    MafiaList<Database::MessageIdType> answerFor = System::Serializer::deserialize<MafiaList<Database::MessageIdType>>(dataCopy);
	Database::ChatFeature feature = System::Serializer::deserialize<Database::ChatFeature>(dataCopy);*/

	try {
		GET_FROM_JSON(Database::ChatFeature, feature, data);
		GET_FROM_JSON(Database::ChatIdType, chatId, data);
		GET_FROM_JSON(QString, messageData, data);
		GET_FROM_JSON(MafiaList<int>, answerFor, data);

		emit MessageProcessor::instance->send_chat_message(sender, chatId, messageData, answerFor, feature);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}


