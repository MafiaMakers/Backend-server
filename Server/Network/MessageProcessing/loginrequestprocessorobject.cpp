#include "loginrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

LoginRequestProcessorObject::LoginRequestProcessorObject(Message message) : ProcessorObject(message){}

void LoginRequestProcessorObject::process()
{
    System::String dataCopy = data;

    QString email = System::Serializer::deserialize<QString>(dataCopy);
    QString password = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->login_user(email, password, sender, id);
}
