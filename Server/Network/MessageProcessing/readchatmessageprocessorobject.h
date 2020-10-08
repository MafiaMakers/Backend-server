#ifndef READCHATMESSAGEPROCESSOROBJECT_H
#define READCHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia{
    namespace Network{
        namespace MessageProcessing {
            class ReadChatMessageProcessorObject : public ProcessorObject
            {
            public:
				ReadChatMessageProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * messageId (int) : id сообщения, которое прочитали
				 */
                void process() override;
            };
        }
    }
}


#endif // READCHATMESSAGEPROCESSOROBJECT_H
