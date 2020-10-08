#ifndef LOGSDATAPROCESSOROBJECT_H
#define LOGSDATAPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class LogsDataProcessorObject : public ProcessorObject
            {
            public:
				LogsDataProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * logsData (String) : данные логов
				 */
                void process() override;
            };
        }
    }
}


#endif // LOGSDATAPROCESSOROBJECT_H
