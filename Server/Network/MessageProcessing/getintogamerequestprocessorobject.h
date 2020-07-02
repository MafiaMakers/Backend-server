#ifndef GETINTOGAMEREQUESTPROCESSOROBJECT_H
#define GETINTOGAMEREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class GetIntoGameRequestProcessorObject : public ProcessorObject
            {
            public:
                GetIntoGameRequestProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // GETINTOGAMEREQUESTPROCESSOROBJECT_H
