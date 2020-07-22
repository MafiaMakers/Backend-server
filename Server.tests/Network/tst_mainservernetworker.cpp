#include "tst_mainservernetworker.h"
#include "Requests/networkrequest.h"
#include "Network/messageTypes.h"
#include <thread>
#include <chrono>

using namespace MafiaTests;
using namespace NetworkTests;

MainServerNetworkerTests::MainServerNetworkerTests(QObject *parent) : QObject(parent)
{
	/*port = Mafia::System::PortsManager::allocate_port();
	networker = new Mafia::Network::MainServerNetworker(10000);

	Mafia::Network::Crypto::setKey("HaHA_UNdecRyPTAble_Key");
	networker->send_message(Mafia::Network::Message(Mafia::Network::MessageType_Text, (char*)"qwert",
													6,
													Mafia::Network::Client(
														QHostAddress("127.0.0.1").toIPv4Address(),
														1000)
													));

	messageReceived = false;
	connect(networker,
			&Mafia::Network::MainServerNetworker::message_received,
			this,
			&MainServerNetworkerTests::on_message_received);
	connect(networker,
			&Mafia::Network::MainServerNetworker::on_subserver_api_message_received,
			this,
			&MafiaTests::NetworkTests::MainServerNetworkerTests::on_message_received);*/
}

void MainServerNetworkerTests::send_receive_message_test()
{
	/*auto message = Mafia::Network::Message(Mafia::Network::MessageType_Text
											(qrand() % Mafia::Network::MessageType_Last),
										   (Mafia::Network::SymbolType*)"This is data",
										   12,
										   Mafia::Network::Client(QHostAddress("127.0.0.1").toIPv4Address(),
																  port)
										   );

	networker->send_message(message);

	int time = 0;
	int timeLimit = 10000;
	while(!messageReceived){
		time++;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		QVERIFY(time <= timeLimit);

		if(time > timeLimit){
			return;
		}
	}
	std::cout << "Received" << std::endl;

	QCOMPARE(message.id, receivedMessage.id);
	QCOMPARE(message.data, receivedMessage.data);
	QCOMPARE(message.size, receivedMessage.size);
	QCOMPARE(message.type, receivedMessage.type);
	QCOMPARE(message.client.ip, receivedMessage.client.ip);
	QCOMPARE(message.client.port, receivedMessage.client.port);
	QCOMPARE(message.partIndex, receivedMessage.partIndex);
	QCOMPARE(message.partsCount, receivedMessage.partsCount);*/

}

void MainServerNetworkerTests::on_message_received(Mafia::Network::Message message)
{
	/*std::cout << "received\n";
	receivedMessage = message;
	messageReceived = true;*/
}
