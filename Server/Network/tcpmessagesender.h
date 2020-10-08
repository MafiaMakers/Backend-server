#ifndef TCPMESSAGESENDER_H
#define TCPMESSAGESENDER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "message.h"

namespace Mafia {
	namespace Network {
		class TCPMessageSender : public QObject
		{
			Q_OBJECT
		public:
			explicit TCPMessageSender(QObject *parent = nullptr);

			TCPMessageSender(int port);

			void send(char* data, int size, Client client);

		signals:

			void message_received(char* data, int size, Client client);

			void on_client_disconnected(Client client);

		private:
			QTcpServer* mainSocket;
			QMap<Client,QTcpSocket *> clientSockets;

		private slots:
			void add_user();

			void receive_message();

			void disconnect_user();

		};
	}
}

#endif // TCPMESSAGESENDER_H
