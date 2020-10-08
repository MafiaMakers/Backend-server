#ifndef CONFIRMEMAILPROCESSOROBJECT_H
#define CONFIRMEMAILPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ConfirmEmailProcessorObject : public ProcessorObject
            {
            public:
				ConfirmEmailProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * confirmationKey (String) : ключ
				 */
                void process() override;
            };
        }
    }
}


#endif // CONFIRMEMAILPROCESSOROBJECT_H
