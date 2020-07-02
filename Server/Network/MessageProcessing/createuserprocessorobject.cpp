#include "createuserprocessorobject.h"
#include "System/serializer.h"
using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

CreateUserProcessorObject::CreateUserProcessorObject(Message message) : ProcessorObject(message){}

void CreateUserProcessorObject::process()
{
    System::String dataCopy = data;
    QString nickname = System::Serializer::deserialize<QString>(dataCopy);
    QString email = System::Serializer::deserialize<QString>(dataCopy);
    QString password = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->create_user(nickname, email, password, sender, id);
}


