#ifndef CREATECHATREQUESTPROCESSOROBJECT_H
#define CREATECHATREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class CreateChatRequestProcessorObject : public ProcessorObject
            {
            public:
                CreateChatRequestProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // CREATECHATREQUESTPROCESSOROBJECT_H
