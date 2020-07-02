#ifndef SENDCHATMESSAGEPROCESSOROBJECT_H
#define SENDCHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class SendChatMessageProcessorObject : public ProcessorObject
            {
            public:
                SendChatMessageProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // SENDCHATMESSAGEPROCESSOROBJECT_H
