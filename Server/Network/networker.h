#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include "message.h"
#include "JSONMessage.h"
#include "tcpmessagesender.h"


typedef QJsonObject KeyValuePairSet;

namespace Mafia {
	namespace Network {
		class Networker : public QObject
		{
			Q_OBJECT
		public:
			explicit Networker(QObject *parent = nullptr);

			Networker(int port);

			void send_message(int id, KeyValuePairSet data, Client receiver);


		signals:
			void message_received(JSONMessage message);
			void subserver_message_received(JSONMessage message);
			void client_disconnected(Client client);


		private slots:
			void on_message_received(char* message, int size, Client client);
			void on_client_disconnected(Client client);

		private:
			void process_message(JSONMessage message);

			QJsonObject KVPSet_to_json(KeyValuePairSet kvpSet);

			TCPMessageSender* mSender;
		};
	}
}

#endif // NETWORKER_H
