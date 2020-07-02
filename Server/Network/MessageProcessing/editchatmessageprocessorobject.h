#ifndef EDITCHATMESSAGEPROCESSOROBJECT_H
#define EDITCHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class EditChatMessageProcessorObject : public ProcessorObject
            {
            public:
                EditChatMessageProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // EDITCHATMESSAGEPROCESSOROBJECT_H
