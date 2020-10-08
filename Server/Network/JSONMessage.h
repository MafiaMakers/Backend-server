#ifndef JSONMESSAGE_H
#define JSONMESSAGE_H

#include <QJsonObject>

#include "message.h"

namespace Mafia {
	namespace Network {
		struct JSONMessage{
			MessageType id;
			Client sender;
			QJsonObject data;
		};

		typedef JSONMessage Message_t;
	}
}

#endif // JSONMESSAGE_H
