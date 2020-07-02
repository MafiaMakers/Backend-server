#include "changenicknameprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ChangeNicknameProcessorObject::ChangeNicknameProcessorObject(Message message) : ProcessorObject(message){}

void ChangeNicknameProcessorObject::process()
{
    System::String dataCopy = data;

    QString newNickname = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->change_nickname(sender, newNickname);
}
