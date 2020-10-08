#ifndef ADDUSERTOCHATPROCESSOROBJECT_H
#define ADDUSERTOCHATPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia{
    namespace Network{
        namespace MessageProcessing {
            class AddUserToChatProcessorObject : public ProcessorObject
            {
            public:
				AddUserToChatProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * userId (int) : id пользователя, с которым работаем
				 * chatId (int) : id чата, с которым работаем
				 * newUsersCapability (int) : новая должность пользователя
				 */
                void process() override;
            };
        }
    }
}


#endif // ADDUSERTOCHATPROCESSOROBJECT_H
