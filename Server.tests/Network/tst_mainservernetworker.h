#ifndef MAINSERVERNETWORKERTESTS_H
#define MAINSERVERNETWORKERTESTS_H

#include "Network/mainservernetworker.h"
#include "System/portsmanager.h"
#include <QTest>
#include <QObject>

namespace MafiaTests {
	namespace NetworkTests {
		class MainServerNetworkerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit MainServerNetworkerTests(QObject *parent = nullptr);

		private slots:
			void send_receive_message_test();
			void on_message_received(Mafia::Network::Message message);
		private:
			Mafia::Network::MainServerNetworker* networker;
			Mafia::Network::Message receivedMessage;
			bool messageReceived;
			int port;
		};
	}
}


#endif // MAINSERVERNETWORKERTESTS_H
