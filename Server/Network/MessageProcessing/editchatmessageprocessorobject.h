#ifndef EDITCHATMESSAGEPROCESSOROBJECT_H
#define EDITCHATMESSAGEPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class EditChatMessageProcessorObject : public ProcessorObject
            {
            public:
				EditChatMessageProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * from (int) : id пользователя, от которого было изначально сообщение
				 * toChat (int) : id чата, в котором это сообщение
				 * feature (int) : особенность сообщения (см. \ref Mafia::Database::ChatFeature)
				 * messageId (int) : id сообщения, которое редактируем
				 * answerFor (List<int>) : список id сообщений, на которые это является ответом
				 * messageData (String) : текст сообщения
				 */
                void process() override;
            };
        }
    }
}


#endif // EDITCHATMESSAGEPROCESSOROBJECT_H
