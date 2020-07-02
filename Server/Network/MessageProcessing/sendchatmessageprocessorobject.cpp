#include "sendchatmessageprocessorobject.h"
#include "System/serializer.h"
using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

SendChatMessageProcessorObject::SendChatMessageProcessorObject(Message message) : ProcessorObject(message){}

void SendChatMessageProcessorObject::process()
{
    System::String dataCopy = data;

    Database::ChatIdType toChat = System::Serializer::deserialize<Database::ChatIdType>(dataCopy);
    QString messageData = System::Serializer::deserialize<QString>(dataCopy);
    MafiaList<Database::MessageIdType> answerFor = System::Serializer::deserialize<MafiaList<Database::MessageIdType>>(dataCopy);
    Database::ChatFeature feature = System::Serializer::deserialize<Database::ChatFeature>(dataCopy);

    emit MessageProcessor::instance->send_chat_message(sender, toChat, messageData, answerFor, feature);
}


