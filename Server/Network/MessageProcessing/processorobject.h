#ifndef PROCESSOROBJECT_H
#define PROCESSOROBJECT_H
#include "Network/messageprocessor.h"
#include "Network/message.h"
#include "Network/messageTypes.h"


namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ProcessorObject
            {
            public:

                static ProcessorObject* generate(Message message);

                virtual void process() = 0;

            protected:
                ProcessorObject(Message message);

                Client sender;
                System::String data;
                MessageIdType id;
            };
        }
    }
}


#endif // PROCESSOROBJECT_H
