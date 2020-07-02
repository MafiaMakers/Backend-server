#ifndef CREATEUSERPROCESSOBJECT_H
#define CREATEUSERPROCESSOBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class CreateUserProcessorObject : public ProcessorObject
            {
            public:
                CreateUserProcessorObject(Message message);

                void process() override;
            };

        }
    }
}

#endif // CREATEUSERPROCESSOBJECT_H
