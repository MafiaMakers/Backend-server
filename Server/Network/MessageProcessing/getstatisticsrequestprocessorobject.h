#ifndef GETSTATISTICSREQUESTPROCESSOROBJECT_H
#define GETSTATISTICSREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class GetStatisticsRequestProcessorObject : public ProcessorObject
            {
            public:
				GetStatisticsRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * userId (int) : id пользователя, статистику которого мы хотим посмотреть
				 */
                void process() override;
            };
        }
    }
}


#endif // GETSTATISTICSREQUESTPROCESSOROBJECT_H
