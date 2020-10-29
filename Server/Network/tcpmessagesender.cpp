#include "tcpmessagesender.h"
#include "Exceptions/messageparsingexception.h"

using namespace Mafia;
using namespace Network;

TCPMessageSender::TCPMessageSender(QObject *parent) : QObject(parent){}

TCPMessageSender::TCPMessageSender(int port) : QObject(nullptr)
{
	mainSocket = new QTcpServer(this);
	bool success = mainSocket->listen(QHostAddress::Any, port);
	if(!success){
		qDebug() << QObject::tr("Unable to start the server: %1.").arg(mainSocket->errorString());
		return;
	}
	connect(mainSocket, &QTcpServer::newConnection, this, &TCPMessageSender::add_user);
	std::cout << "Server initialized on port " << port << std::endl;
}

void TCPMessageSender::send(char* data, int size, Client client)
{
	if(clientSockets.contains(client)){
		QTextStream output(clientSockets[client]);
		output.setAutoDetectUnicode(true);
		output << QString::fromStdString(std::string(data, size));
		output.flush();
	} else {
		throw Exceptions::Exception::generate(System::String("No connection with this client!"),
											  Exceptions::MessageParsingExceptionId_NoClientConnection);
	}
}

void TCPMessageSender::add_user()
{
	//qDebug() << "adding new client\n";
	QTcpSocket* clientSocket = mainSocket->nextPendingConnection();
	//int userSocketDescription=clientSocket->socketDescriptor();
	Client c = Client(clientSocket->peerAddress().toIPv4Address(), clientSocket->peerPort());
	clientSockets[ c ] = clientSocket;
	connect(clientSocket, &QTcpSocket::readyRead,this, &TCPMessageSender::receive_message);
	connect(clientSocket, &QTcpSocket::disconnected, this, &TCPMessageSender::disconnect_user);
}

void TCPMessageSender::receive_message()
{
	//std::cout << "Message received!\n";
	QTcpSocket* clientSocket = ( QTcpSocket* )sender();

	QByteArray data = clientSocket->readAll();

	int port = clientSocket->peerPort();
	int ip = clientSocket->peerAddress().toIPv4Address();

	emit message_received(data.data(), data.size(), Client(ip, port));
}

void TCPMessageSender::disconnect_user()
{
	QTcpSocket* clientSocket = ( QTcpSocket* )sender();

	int port = clientSocket->peerPort();
	int ip = clientSocket->peerAddress().toIPv4Address();

	Client disconnected = Client(ip, port);

	emit on_client_disconnected(disconnected);

	delete clientSockets[ disconnected ];
	clientSockets.remove( disconnected );
}
