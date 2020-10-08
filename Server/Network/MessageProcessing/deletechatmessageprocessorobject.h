#ifndef DELETECHATMESSAGEPROCESSOROBJECT_H
#define DELETECHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia{
    namespace Network {
        namespace MessageProcessing {
            class DeleteChatMessageProcessorObject : public ProcessorObject
            {
            public:
				DeleteChatMessageProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * messageId (int) : id сообщения, которое удаляем
				 */
                void process() override;
            };
        }
    }
}


#endif // DELETECHATMESSAGEPROCESSOROBJECT_H
