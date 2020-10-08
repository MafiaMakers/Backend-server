#ifndef CREATEUSERPROCESSOBJECT_H
#define CREATEUSERPROCESSOBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class CreateUserProcessorObject : public ProcessorObject
            {
            public:
				CreateUserProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * nickname (String) : никнейм
				 * email (String) : email
				 * password (String) : пароль
				 */
                void process() override;
            };

        }
    }
}

#endif // CREATEUSERPROCESSOBJECT_H
