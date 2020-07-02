#include "getstatisticsrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

GetStatisticsRequestProcessorObject::GetStatisticsRequestProcessorObject(Message message) : ProcessorObject(message){}

void GetStatisticsRequestProcessorObject::process()
{
    System::String dataCopy = data;

    Database::UserIdType userId = System::Serializer::deserialize<Database::UserIdType>(dataCopy);

    emit MessageProcessor::instance->get_statistics(userId, sender, id);
}
