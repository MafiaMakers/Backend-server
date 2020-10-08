#ifndef GETCHATMESSAGESPROCESSOROBJECT_H
#define GETCHATMESSAGESPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class GetChatMessagesProcessorObject : public ProcessorObject
            {
            public:
				GetChatMessagesProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * messagesCount (int) : количество сообщений, которое стоит загрузить (если указать -1, то загрузится стандартное количество)
				 * chatId (int) : id чата, из которого загружать
				 */
                void process() override;
            };
        }
    }
}


#endif // GETCHATMESSAGESPROCESSOROBJECT_H
