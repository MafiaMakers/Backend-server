#include "getstatisticsrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

GetStatisticsRequestProcessorObject::GetStatisticsRequestProcessorObject(Message_t message) : ProcessorObject(message){}

void GetStatisticsRequestProcessorObject::process()
{
	/*System::String dataCopy = data;

    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);

	emit MessageProcessor::instance->get_statistics(userId, sender, id);*/

	try {
		GET_FROM_JSON(Database::UserIdType, userId, data);

		emit MessageProcessor::instance->get_statistics(userId, sender/*, id*/);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
