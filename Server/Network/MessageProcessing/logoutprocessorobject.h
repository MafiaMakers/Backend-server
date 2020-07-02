#ifndef LOGOUTPROCESSOROBJECT_H
#define LOGOUTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing  {
            class LogoutProcessorObject : public ProcessorObject
            {
            public:
                LogoutProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // LOGOUTPROCESSOROBJECT_H
