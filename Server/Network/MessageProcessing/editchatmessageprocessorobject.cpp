#include "editchatmessageprocessorobject.h"
#include "System/serializer.h"
using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

EditChatMessageProcessorObject::EditChatMessageProcessorObject(Message_t message)
    : ProcessorObject(message){}

void EditChatMessageProcessorObject::process()
{
	/*System::String dataCopy = data;


    Database::ChatMessage chatMessage = Database::ChatMessage();
    chatMessage.id = System::Serializer::deserialize<Database::MessageIdType>(dataCopy);
    chatMessage.data = System::Serializer::deserialize<QString>(dataCopy);
    chatMessage.from = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
    chatMessage.toChat = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    chatMessage.answerFor = System::Serializer::deserialize<MafiaList<Database::MessageIdType>>(dataCopy);
	chatMessage.feature = System::Serializer::deserialize<Database::ChatFeature>(dataCopy);*/
	try {
		auto chatMessage = Database::ChatMessage();

		GET_FROM_JSON(Database::MessageIdType, messageId, data);
		GET_FROM_JSON(QString, messageData, data);
		GET_FROM_JSON(Database::UserIdType, from, data);
		GET_FROM_JSON(Database::ChatIdType, toChat, data);
		GET_FROM_JSON(MafiaList<Database::MessageIdType>, answerFor, data);
		GET_FROM_JSON_T(Database::ChatFeature, feature, int, data);

		chatMessage.id = messageId;
		chatMessage.data = messageData;
		chatMessage.from = from;
		chatMessage.toChat = toChat;
		chatMessage.feature = feature;
		chatMessage.answerFor = answerFor;

		emit MessageProcessor::instance->edit_message(sender, chatMessage);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}

}
