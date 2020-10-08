#include "getintogamerequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

GetIntoGameRequestProcessorObject::GetIntoGameRequestProcessorObject(Message_t message) : ProcessorObject(message){}

void GetIntoGameRequestProcessorObject::process()
{
	/*System::String dataCopy = data;

    QString gameKey = System::Serializer::deserialize<QString>(dataCopy);

	emit MessageProcessor::instance->add_user_to_game(sender, gameKey);*/

	try {
		GET_FROM_JSON(QString, gameKey, data);

		emit MessageProcessor::instance->add_user_to_game(sender, gameKey);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
