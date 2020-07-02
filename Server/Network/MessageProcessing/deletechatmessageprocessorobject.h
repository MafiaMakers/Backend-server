#ifndef DELETECHATMESSAGEPROCESSOROBJECT_H
#define DELETECHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia{
    namespace Network {
        namespace MessageProcessing {
            class DeleteChatMessageProcessorObject : public ProcessorObject
            {
            public:
                DeleteChatMessageProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // DELETECHATMESSAGEPROCESSOROBJECT_H
