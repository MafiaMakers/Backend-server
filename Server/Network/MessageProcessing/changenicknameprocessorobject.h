#ifndef CHANGENICKNAMEPROCESSOROBJECT_H
#define CHANGENICKNAMEPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ChangeNicknameProcessorObject : public ProcessorObject
            {
            public:
                ChangeNicknameProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // CHANGENICKNAMEPROCESSOROBJECT_H
