#include "creategamerequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

CreateGameRequestProcessorObject::CreateGameRequestProcessorObject(Message message) : ProcessorObject(message){}

void CreateGameRequestProcessorObject::process()
{
    emit MessageProcessor::instance->create_game(sender);
}
