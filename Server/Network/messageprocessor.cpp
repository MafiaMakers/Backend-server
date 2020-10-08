#include "messageprocessor.h"
#include <MafiaExceptions>
#include "MessageProcessing/processorobject.h"
#include "Network/networker.h"

using namespace Mafia;
using namespace Network;

MessageProcessor* MessageProcessor::instance = NULL;

MessageProcessor::MessageProcessor(QObject *parent) : QObject(parent){}

MessageProcessor::MessageProcessor(Network::Networker *networker)
{
    this->networker = networker;

    instance = this;

	connect(this, &MessageProcessor::message_received, networker, &Network::Networker::message_received);
}

void MessageProcessor::message_received(Message_t message)
{
	MessageProcessing::ProcessorObject* processor =
		MessageProcessing::ProcessorObject::generate(message);

	processor->process();
	SAFE_DELETE(processor);
}
