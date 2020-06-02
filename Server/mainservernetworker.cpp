#include <iostream>
#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include <QUdpSocket>
#include <QtCore>
using namespace Mafia;

MainServerNetworker::MainServerNetworker(int port)
{
    this->socket = new QUdpSocket(this);
    this->myPort = port;
    this->socket->bind(QHostAddress::Any, this->myPort);
    connect(socket, SIGNAL(readyRead()), this, SLOT(receive_message()));
}

void MainServerNetworker::send_message(char* data, int size, QHostAddress client) {
    socket->writeDatagram(data, size, client, 20001 - this->myPort);
    //std::cout << "sent " << str.toStdString() << std::endl;
}

void MainServerNetworker::receive_message() {
    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());

    QHostAddress *address = new QHostAddress();
    quint16 port;
    auto s = socket->readDatagram(datagram.data(), datagram.size(), address, &port);
    std::cout << "Received a message\nsize - " << s << std::endl;
    Message trueData;
    try {
        trueData = Crypto::parse_data(datagram.data(), datagram.size());
    } catch (Exception* exception) {
        exception->show();
    }
    std::cout << "Message : \n" << "    id : " << trueData.id << "\n    type : " << trueData.type << "\n    data : " << trueData.data << std::endl;
    //std::cout << datagram.data() << std::endl;
    int ip = address->toIPv4Address();
    unsigned char* ip_char = (unsigned char*)&ip;
    std::cout << "from ";
    for(int i = 0; i < 4; i++){
        std::cout << (int)ip_char[3-i];
        if(i < 3){
            std::cout << ".";
        }
    }

    std::cout << std::endl;
    std::cout << "port " << port << std::endl;
}
