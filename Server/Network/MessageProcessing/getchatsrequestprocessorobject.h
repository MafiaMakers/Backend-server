#ifndef GETCHATSREQUESTPROCESSOROBJECT_H
#define GETCHATSREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia{
    namespace Network {
        namespace MessageProcessing {
            class GetChatsRequestProcessorObject : public ProcessorObject
            {
            public:
                GetChatsRequestProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // GETCHATSREQUESTPROCESSOROBJECT_H
