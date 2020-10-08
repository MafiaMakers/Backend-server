#ifndef GETINTOGAMEREQUESTPROCESSOROBJECT_H
#define GETINTOGAMEREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class GetIntoGameRequestProcessorObject : public ProcessorObject
            {
            public:
				GetIntoGameRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * gameKey (String) : ключ игры
				 */
                void process() override;
            };
        }
    }
}


#endif // GETINTOGAMEREQUESTPROCESSOROBJECT_H
