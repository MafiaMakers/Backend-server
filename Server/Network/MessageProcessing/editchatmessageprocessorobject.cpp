#include "editchatmessageprocessorobject.h"
#include "System/serializer.h"

Mafia::Network::MessageProcessing::EditChatMessageProcessorObject::EditChatMessageProcessorObject(Mafia::Network::Message message)
    : ProcessorObject(message){}

void Mafia::Network::MessageProcessing::EditChatMessageProcessorObject::process()
{
    System::String dataCopy = data;


    Database::ChatMessage chatMessage = Database::ChatMessage();
    chatMessage.id = System::Serializer::deserialize<Database::MessageIdType>(dataCopy);
    chatMessage.data = System::Serializer::deserialize<QString>(dataCopy);
    chatMessage.from = System::Serializer::deserialize<Database::UserIdType>(dataCopy);
    chatMessage.toChat = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    chatMessage.answerFor = System::Serializer::deserialize<MafiaList<Database::MessageIdType>>(dataCopy);
    chatMessage.feature = System::Serializer::deserialize<Database::ChatFeature>(dataCopy);

    emit MessageProcessor::instance->edit_message(sender, chatMessage);
}
