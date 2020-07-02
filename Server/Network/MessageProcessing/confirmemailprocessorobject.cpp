#include "confirmemailprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ConfirmEmailProcessorObject::ConfirmEmailProcessorObject(Message message) : ProcessorObject(message){}

void ConfirmEmailProcessorObject::process()
{
    System::String dataCopy = data;

    QString confirmationKey = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->confirm_email(sender, confirmationKey, id);
}
