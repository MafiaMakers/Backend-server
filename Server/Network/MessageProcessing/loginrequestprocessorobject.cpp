#include "loginrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

LoginRequestProcessorObject::LoginRequestProcessorObject(Message_t message) : ProcessorObject(message){}

void LoginRequestProcessorObject::process()
{
	/*System::String dataCopy = data;

    QString email = System::Serializer::deserialize<QString>(dataCopy);
    QString password = System::Serializer::deserialize<QString>(dataCopy);

	emit MessageProcessor::instance->login_user(email, password, sender, id);*/

	try {
		GET_FROM_JSON(QString, email, data);
		GET_FROM_JSON(QString, password, data);

		emit MessageProcessor::instance->login_user(email, password, sender/*, id*/);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
