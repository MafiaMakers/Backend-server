#ifndef CONFIRMEMAILPROCESSOROBJECT_H
#define CONFIRMEMAILPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ConfirmEmailProcessorObject : public ProcessorObject
            {
            public:
                ConfirmEmailProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // CONFIRMEMAILPROCESSOROBJECT_H
