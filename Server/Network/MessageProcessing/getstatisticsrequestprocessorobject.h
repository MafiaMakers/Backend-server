#ifndef GETSTATISTICSREQUESTPROCESSOROBJECT_H
#define GETSTATISTICSREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class GetStatisticsRequestProcessorObject : public ProcessorObject
            {
            public:
                GetStatisticsRequestProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // GETSTATISTICSREQUESTPROCESSOROBJECT_H
