#include "getchatsrequestprocessorobject.h"
#include "System/serializer.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;


GetChatsRequestProcessorObject::GetChatsRequestProcessorObject(Message_t message) : ProcessorObject(message){}

void GetChatsRequestProcessorObject::process()
{
	/*System::String dataCopy = data;
    int chatsCount = System::Serializer::deserialize<int>(dataCopy);

	emit MessageProcessor::instance->get_users_chats(sender, id, chatsCount);*/

	try {
		GET_FROM_JSON(int, chatsCount, data);

		emit MessageProcessor::instance->get_users_chats(sender/*, id*/, chatsCount);
	} catch (Exceptions::Exception* ex) {
		ex->close();
	}
}
