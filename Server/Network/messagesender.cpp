#include "messagesender.h"
#include "System/functions.h"

Mafia::Network::MessageSender::MessageSender(int port) : myPort(port)
{
	this->socket = new QUdpSocket(this);
	this->socket->bind(QHostAddress::Any, this->myPort);
	connect(socket, &QUdpSocket::readyRead, this, &MessageSender::receive_message);
}

void Mafia::Network::MessageSender::send(char* message, unsigned int size, QString ip, int port)
{
	socket->writeDatagram(message, size, QHostAddress(ip), port);
}

void Mafia::Network::MessageSender::send(char* message, unsigned int size, int ip, int port)
{
	socket->writeDatagram(message, size, QHostAddress(ip), port);
}

void Mafia::Network::MessageSender::receive_message()
{

	//Получаем сообщение из сокета
	QByteArray datagram;
	datagram.resize(socket->pendingDatagramSize());

	QHostAddress *address;
	SAFE_NEW(address, QHostAddress());
	quint16 port;
	auto bytesReceived = socket->readDatagram(datagram.data(), datagram.size(), address, &port);

	emit message_received(datagram.data(), bytesReceived, address->toIPv4Address(), port);
	SAFE_DELETE(address);
}
