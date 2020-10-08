#include "logoutprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

LogoutProcessorObject::LogoutProcessorObject(Message_t message) : ProcessorObject(message){}

void LogoutProcessorObject::process()
{
    emit MessageProcessor::instance->logout_user(sender);
}
