#ifndef CREATEGAMEREQUESTPROCESSOROBJECT_H
#define CREATEGAMEREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class CreateGameRequestProcessorObject : public ProcessorObject
            {
            public:
				CreateGameRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * (ничего)
				 */
                void process() override;
            };
        }
    }
}


#endif // CREATEGAMEREQUESTPROCESSOROBJECT_H
