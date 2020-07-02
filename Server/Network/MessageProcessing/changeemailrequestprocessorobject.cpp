#include "changeemailrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ChangeEmailRequestProcessorObject::ChangeEmailRequestProcessorObject(Message message) : ProcessorObject(message){}

void ChangeEmailRequestProcessorObject::process()
{
    System::String dataCopy;

    QString newEmail = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->change_email(sender, newEmail, id);
}
