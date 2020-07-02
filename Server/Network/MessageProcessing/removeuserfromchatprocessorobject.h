#ifndef REMOVEUSERFROMCHATPROCESSOROBJECT_H
#define REMOVEUSERFROMCHATPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class RemoveUserFromChatProcessorObject : public ProcessorObject
            {
            public:
                RemoveUserFromChatProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // REMOVEUSERFROMCHATPROCESSOROBJECT_H
