#ifndef SENDCHATMESSAGEPROCESSOROBJECT_H
#define SENDCHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class SendChatMessageProcessorObject : public ProcessorObject
            {
            public:
				SendChatMessageProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * toChat (int) : id чата, в котором это сообщение
				 * feature (int) : особенность сообщения (см. \ref Mafia::Database::ChatFeature)
				 * answerFor (List<int>) : список id сообщений, на которые это является ответом
				 * messageData (String) : текст сообщения
				 */
                void process() override;
            };
        }
    }
}


#endif // SENDCHATMESSAGEPROCESSOROBJECT_H
