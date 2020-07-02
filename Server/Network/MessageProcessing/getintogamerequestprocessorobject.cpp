#include "getintogamerequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

GetIntoGameRequestProcessorObject::GetIntoGameRequestProcessorObject(Message message) : ProcessorObject(message){}

void GetIntoGameRequestProcessorObject::process()
{
    System::String dataCopy = data;

    QString gameKey = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->add_user_to_game(sender, gameKey);
}
