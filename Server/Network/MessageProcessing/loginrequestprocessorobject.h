#ifndef LOGINREQUESTPROCESSOROBJECT_H
#define LOGINREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class LoginRequestProcessorObject : public ProcessorObject
            {
            public:
                LoginRequestProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // LOGINREQUESTPROCESSOROBJECT_H
