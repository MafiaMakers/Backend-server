#ifndef REMOVEUSERFROMCHATPROCESSOROBJECT_H
#define REMOVEUSERFROMCHATPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class RemoveUserFromChatProcessorObject : public ProcessorObject
            {
            public:
				RemoveUserFromChatProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * userId (int) : id пользователя, которого удаляем
				 * chatId (int) : id чата, в котором это происходит
				 */
                void process() override;
            };
        }
    }
}


#endif // REMOVEUSERFROMCHATPROCESSOROBJECT_H
