#include "tst_tcpmessagesender.h"
#include "System/portsmanager.h"
#include "System/keygen.h"
#include "QDataStream"
#include "thread"
#include "chrono"

using namespace MafiaTests;
using namespace NetworkTests;

TCPMessageSenderTests::TCPMessageSenderTests(QObject *parent) : QObject(parent)
{
	/*int port = 5500;//System::PortsManager::allocate_port();

	sender = new TCPMessageSender(port);
	socket = new QTcpSocket(this);
	qDebug() << "Starting\n";

	//socket->bind(QHostAddress::LocalHost, port);
	socket->connectToHost(QHostAddress::LocalHost, port);
	qDebug() << "socket connected\n";

	connect(socket, &QTcpSocket::readyRead, this, &TCPMessageSenderTests::readyRead);
	connect(socket, &QTcpSocket::connected, this, &TCPMessageSenderTests::connected);

	connect(sender, &TCPMessageSender::message_received, this, &TCPMessageSenderTests::message_received);
	connect(sender, &TCPMessageSender::on_client_disconnected, this, &TCPMessageSenderTests::on_client_disconnected);

	me.ip = QHostAddress("127.0.0.1").toIPv4Address();
	me.port = socket->peerPort();*/
}

void TCPMessageSenderTests::message_received(char* data, int size, Client client)
{
	/*lastMessage = QString::fromStdString(std::string(data, size));
	lastClient = client;
	this->signalReceived = true;*/
}

void TCPMessageSenderTests::on_client_disconnected(Client client)
{
	/*lastClient = client;
	this->signalReceived = true;*/
}

void TCPMessageSenderTests::readyRead()
{
	/*QByteArray data = socket->readAll();
	lastClient.ip = -1;
	lastClient.port = -1;
	lastMessage = QString(data);
	signalReceived = true;*/
}

void TCPMessageSenderTests::connected()
{
	/*signalReceived = true;*/
}

void TCPMessageSenderTests::error(QAbstractSocket::SocketError error)
{
	/*std::cout << "Socket error " << error << "!" << std::endl;*/
}

void TCPMessageSenderTests::send_to_client_test()
{
	/*std::string data = "This is test message!!!";

	signalReceived = false;
	lastMessage = "";
	lastClient = Client();
	qDebug() << "sending\n";
	try {
		sender->send((char*)data.c_str(), data.length(), me);
	} catch (Exceptions::Exception* exception) {
		QString data = QString::fromStdString(std::string(exception->get_data().data, exception->get_data().size));
		data += ": " + QString::number(exception->get_id());
		QFAIL(data.toStdString().c_str());
	}

	qDebug() << "sent\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	if(signalReceived){
		QCOMPARE(QString::fromStdString(data), lastMessage);
		QCOMPARE(lastClient.ip, -1);
		QCOMPARE(lastClient.port, -1);
	} else{
		QFAIL("Message is not received!");
	}*/
}

void TCPMessageSenderTests::send_to_client_random_test()
{
	/*std::string data = System::KeyGen::generate_key<std::string>(20);

	signalReceived = false;
	lastMessage = "";
	lastClient = Client();

	qDebug() << "sending\n";
	try {
		sender->send((char*)data.c_str(), data.length(), me);
	} catch (Exceptions::Exception* exception) {
		QString data = QString::fromStdString(std::string(exception->get_data().data, exception->get_data().size));
		data += ": " + QString::number(exception->get_id());
		QFAIL(data.toStdString().c_str());
	}

	qDebug() << "sent\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	if(signalReceived){
		QCOMPARE(QString::fromStdString(data), lastMessage);
		QCOMPARE(lastClient.ip, -1);
		QCOMPARE(lastClient.port, -1);
	} else{
		QFAIL("Message is not received!");
	}*/
}

void TCPMessageSenderTests::send_to_server_test()
{
	/*signalReceived = false;
	lastMessage = "";
	lastClient = Client();

	std::string data = "Some test message";

	socket->write((char*)data.c_str(), data.length());
	//socket->flush();

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	if(signalReceived){
		QCOMPARE(QString::fromStdString(data), lastMessage);
		QCOMPARE(lastClient.ip, me.ip);
		QCOMPARE(lastClient.port, me.port);
	} else{
		QFAIL("Message is not received!");
	}*/
}

void TCPMessageSenderTests::send_to_server_random_test()
{
	/*signalReceived = false;
	lastMessage = "";
	lastClient = Client();

	std::string data = System::KeyGen::generate_key<std::string>(20);

	socket->write((char*)data.c_str(), data.length());
	//socket->flush();

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	if(signalReceived){
		QCOMPARE(QString::fromStdString(data), lastMessage);
		QCOMPARE(lastClient.ip, me.ip);
		QCOMPARE(lastClient.port, me.port);
	} else{
		QFAIL("Message is not received!");
	}*/
}

void TCPMessageSenderTests::disconnect_client()
{
	/*signalReceived = false;
	lastMessage = "";
	lastClient = Client();

	socket->close();

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	if(signalReceived){
		QCOMPARE(lastClient.ip, me.ip);
		QCOMPARE(lastClient.port, me.port);
	} else{
		QFAIL("Signal is not received!");
	}*/
}
