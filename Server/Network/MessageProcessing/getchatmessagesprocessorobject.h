#ifndef GETCHATMESSAGESPROCESSOROBJECT_H
#define GETCHATMESSAGESPROCESSOROBJECT_H
#include "processorobject.h"


namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class GetChatMessagesProcessorObject : public ProcessorObject
            {
            public:
                GetChatMessagesProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // GETCHATMESSAGESPROCESSOROBJECT_H
