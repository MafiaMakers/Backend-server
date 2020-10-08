#ifndef CHANGENICKNAMEPROCESSOROBJECT_H
#define CHANGENICKNAMEPROCESSOROBJECT_H
#include "processorobject.h"

namespace Mafia {
    namespace Network {
        namespace MessageProcessing {
            class ChangeNicknameProcessorObject : public ProcessorObject
            {
            public:
				ChangeNicknameProcessorObject(Message_t message);

				/*!
				 * \brief id, которые используются:
				 * newNickname (String) : новый никнейм
				 */
                void process() override;
            };
        }
    }
}


#endif // CHANGENICKNAMEPROCESSOROBJECT_H
