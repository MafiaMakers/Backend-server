#ifndef READCHATMESSAGEPROCESSOROBJECT_H
#define READCHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia{
    namespace Network{
        namespace MessageProcessing {
            class ReadChatMessageProcessorObject : public ProcessorObject
            {
            public:
                ReadChatMessageProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // READCHATMESSAGEPROCESSOROBJECT_H
