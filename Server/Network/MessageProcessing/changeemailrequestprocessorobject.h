#ifndef CHANGEEMAILREQUESTPROCESSOROBJECT_H
#define CHANGEEMAILREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ChangeEmailRequestProcessorObject : public ProcessorObject
            {
            public:
				ChangeEmailRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * newEmail (String) : новый email
				 */
                void process() override;
            };
        }
    }
}


#endif // CHANGEEMAILREQUESTPROCESSOROBJECT_H
