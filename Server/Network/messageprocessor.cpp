#include "messageprocessor.h"
#include <MafiaExceptions>
#include "MessageProcessing/processorobject.h"

using namespace Mafia;
using namespace Network;

MessageProcessor* MessageProcessor::instance = NULL;

MessageProcessor::MessageProcessor(QObject *parent) : QObject(parent){}

MessageProcessor::MessageProcessor(Network::MainServerNetworker *networker)
{
    this->networker = networker;

    instance = this;

    connect(this, &MessageProcessor::message_received, networker, &Network::MainServerNetworker::message_received);
}

void MessageProcessor::message_received(Network::Message message)
{
    MessageProcessing::ProcessorObject::generate(message)->process();
}
