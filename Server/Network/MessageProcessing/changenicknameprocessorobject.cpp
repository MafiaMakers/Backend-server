#include "changenicknameprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ChangeNicknameProcessorObject::ChangeNicknameProcessorObject(Message_t message) : ProcessorObject(message){}

void ChangeNicknameProcessorObject::process()
{
	/*System::String dataCopy = data;

    QString newNickname = System::Serializer::deserialize<QString>(dataCopy);

	emit MessageProcessor::instance->change_nickname(sender, newNickname);*/

	try {
		GET_FROM_JSON(QString, newNickname, data);

		emit MessageProcessor::instance->change_nickname(sender, newNickname);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
