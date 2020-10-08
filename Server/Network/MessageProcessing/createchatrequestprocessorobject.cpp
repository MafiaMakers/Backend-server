#include "createchatrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

CreateChatRequestProcessorObject::CreateChatRequestProcessorObject(Message_t message) : ProcessorObject(message){}

void CreateChatRequestProcessorObject::process()
{
    emit MessageProcessor::instance->create_chat(sender, id);
}
