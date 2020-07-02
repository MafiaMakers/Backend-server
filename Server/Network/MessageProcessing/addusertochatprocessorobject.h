#ifndef ADDUSERTOCHATPROCESSOROBJECT_H
#define ADDUSERTOCHATPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia{
    namespace Network{
        namespace MessageProcessing {
            class AddUserToChatProcessorObject : public ProcessorObject
            {
            public:
                AddUserToChatProcessorObject(Message message);

                void process() override;
            };
        }
    }
}


#endif // ADDUSERTOCHATPROCESSOROBJECT_H
