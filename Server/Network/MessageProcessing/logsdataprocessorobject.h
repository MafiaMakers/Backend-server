#ifndef LOGSDATAPROCESSOROBJECT_H
#define LOGSDATAPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class LogsDataProcessorObject : public ProcessorObject
            {
            public:
                LogsDataProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // LOGSDATAPROCESSOROBJECT_H
