#ifndef LOGINREQUESTPROCESSOROBJECT_H
#define LOGINREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class LoginRequestProcessorObject : public ProcessorObject
            {
            public:
				LoginRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * email (String) : email
				 * password (String) : пароль
				 */
                void process() override;
            };
        }
    }
}


#endif // LOGINREQUESTPROCESSOROBJECT_H
