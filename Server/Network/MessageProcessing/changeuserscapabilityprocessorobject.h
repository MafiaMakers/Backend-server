#ifndef CHANGEUSERSCAPABILITYPROCESSOROBJECT_H
#define CHANGEUSERSCAPABILITYPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ChangeUsersCapabilityProcessorObject : public ProcessorObject
            {
            public:
				ChangeUsersCapabilityProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * userId (int) : id пользователя, с которым работаем
				 * chatId (int) : id чата, с которым работаем
				 * newCapability (int) : новая должность пользователя
				 */
                void process() override;
            };
        }
    }
}


#endif // CHANGEUSERSCAPABILITYPROCESSOROBJECT_H
