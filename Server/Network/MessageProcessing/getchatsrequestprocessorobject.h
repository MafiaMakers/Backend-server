#ifndef GETCHATSREQUESTPROCESSOROBJECT_H
#define GETCHATSREQUESTPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia{
    namespace Network {
        namespace MessageProcessing {
            class GetChatsRequestProcessorObject : public ProcessorObject
            {
            public:
				GetChatsRequestProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * chatsCount (int) : количество чатов, которые стоит загружать (если указать -1, то загрузится стандартное кол-во)
				 */
                void process() override;
            };
        }
    }
}


#endif // GETCHATSREQUESTPROCESSOROBJECT_H
