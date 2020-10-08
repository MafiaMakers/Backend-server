#ifndef CREATECHATREQUESTPROCESSOROBJECT_H
#define CREATECHATREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class CreateChatRequestProcessorObject : public ProcessorObject
            {
            public:
				CreateChatRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * (ничего)
				 */
                void process() override;
            };
        }
    }
}


#endif // CREATECHATREQUESTPROCESSOROBJECT_H
