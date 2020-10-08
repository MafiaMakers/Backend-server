#include "networker.h"
#include "QJsonDocument"
#include "Exceptions/messageparsingexception.h"
#include "System/functions.h"

using namespace Mafia;
using namespace Network;

Networker::Networker(QObject *parent) : QObject(parent){}

Networker::Networker(int port)
{
	this->mSender = new TCPMessageSender(port);
	connect(mSender, &TCPMessageSender::message_received, this, &Networker::on_message_received);
	connect(mSender, &TCPMessageSender::on_client_disconnected, this, &Networker::on_client_disconnected);
}

void Networker::send_message(int id, KeyValuePairSet data, Client receiver)
{
	QJsonObject messageObj = QJsonObject();
	messageObj.insert("id", id);
	messageObj.insert("data", KVPSet_to_json(data));

	std::string strData = QJsonDocument(messageObj).toJson().toStdString();

	mSender->send((char*)strData.data(), strData.length(), receiver);
}

void Networker::on_message_received(char* message, int size, Client client)
{
	QByteArray data = QByteArray::fromStdString(std::string(message, size));
	QJsonDocument doc = QJsonDocument::fromJson(data);
	if(doc.isNull()){
		Exceptions::Exception::process_uncatchable_exception(System::String("JSON parsing error!"),
										Exceptions::MessageParsingExceptionId_JSONError);
		return;
	}
	QJsonObject json = doc.object();

	JSONMessage mes = JSONMessage();
	mes.id = json.value("id").toInt();
	mes.sender = client;
	mes.data = json.value("data").toObject();

	process_message(mes);
}

void Networker::on_client_disconnected(Client client)
{
	emit client_disconnected(client);
}

void Networker::process_message(JSONMessage message)
{
	emit message_received(message);
	if((unsigned)message.sender.ip == QHostAddress("127.0.0.1").toIPv4Address()){
		emit subserver_message_received(message);
	}
}

QJsonObject Networker::KVPSet_to_json(KeyValuePairSet kvpSet)
{
	return kvpSet;
}
