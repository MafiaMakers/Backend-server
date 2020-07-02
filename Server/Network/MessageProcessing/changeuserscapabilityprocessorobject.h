#ifndef CHANGEUSERSCAPABILITYPROCESSOROBJECT_H
#define CHANGEUSERSCAPABILITYPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ChangeUsersCapabilityProcessorObject : public ProcessorObject
            {
            public:
                ChangeUsersCapabilityProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // CHANGEUSERSCAPABILITYPROCESSOROBJECT_H
