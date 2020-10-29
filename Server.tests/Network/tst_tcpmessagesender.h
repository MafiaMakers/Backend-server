#ifndef TCPMESSAGESENDERTESTS_H
#define TCPMESSAGESENDERTESTS_H

#include <QObject>
#include "Network/tcpmessagesender.h"
#include "QTest"
using namespace Mafia;
using namespace Network;

namespace MafiaTests {
	namespace NetworkTests {

		class TCPMessageSenderTests : public QObject
		{
			Q_OBJECT
		public:
			explicit TCPMessageSenderTests(QObject *parent = nullptr);

		private slots:
			void message_received(char* data, int size, Client client);

			void on_client_disconnected(Client client);

			void readyRead();
			void connected();
			void error(QAbstractSocket::SocketError error);

			void send_to_client_test();
			void send_to_client_random_test();

			void send_to_server_test();
			void send_to_server_random_test();

			void disconnect_client();

		private:
			TCPMessageSender* sender;
			QTcpSocket * socket;

			QString lastMessage = "";
			Client lastClient = Client();

			Client me = Client();

			bool signalReceived = false;
		};
	}
}

#endif // TCPMESSAGESENDERTESTS_H
