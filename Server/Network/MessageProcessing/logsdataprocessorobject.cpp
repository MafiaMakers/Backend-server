#include "logsdataprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

LogsDataProcessorObject::LogsDataProcessorObject(Message message) : ProcessorObject(message){}

void LogsDataProcessorObject::process()
{
    System::String dataCopy = data;

    QString logsData = System::Serializer::deserialize<QString>(dataCopy);

    emit MessageProcessor::instance->get_logs_data(logsData, sender);
}
