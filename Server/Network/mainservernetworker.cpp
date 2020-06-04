#include <iostream>
#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include <QUdpSocket>
#include <QtCore>
#include <QSet>
#include "messageTypes.h"
#include "Exceptions/messageprocessingexception.h"


using namespace Mafia;

const int MainServerNetworker::timeToResend = 500;
const int MainServerNetworker::maxResendCount = 100;
//const int MainServerNetworker::idsForClient = 1000;

const QSet<MessageTypeType> MainServerNetworker::needConfirmation = QSet<MessageTypeType>()
        << TEXT_MESSAGE_TYPE
        << REQUEST_ANSWER_MESSAGE_TYPE
        << ABSTRACT_REQUEST_MESSAGE_TYPE;

MainServerNetworker::MainServerNetworker(int port)
{
    this->currentMaxId = 0;
    this->socket = new QUdpSocket(this);
    this->myPort = port;
    this->socket->bind(QHostAddress::Any, this->myPort);
    this->waitingForConfirmation = QList<Message>();
    std::thread resendingThread(&MainServerNetworker::_resend_not_confirmed_messages, this);
    resendingThread.detach();
    connect(socket, SIGNAL(readyRead()), this, SLOT(receive_message()));
}

MessageIdType MainServerNetworker::send_message(Message message)
{
    String mes = String();
    if(message.id == (MessageIdType)(0)){
        currentMaxId++;
        message.id = currentMaxId;
    }
    try {
        mes = Crypto::wrap_message(message);
    } catch (Exception* exception) {
        exception->show();
        return -2;
    }

    if(needConfirmation.contains(message.type)){
        waitingForConfirmation.append(message);
    }

    _send_message(mes.data, mes.size, QHostAddress(message.client.ip), message.client.port);
    return message.id;
    //std::cout << "Sent a message" << std::endl;
}

void MainServerNetworker::_send_message(char* data, int size, QHostAddress client, int port) {
    //std::cout << "Sent " << data << " of size " << size << " to client with ip " << client.toString().toStdString() << " to port " << port << std::endl;
    socket->writeDatagram(data, size, client, port);
}

void MainServerNetworker::_process_message(Message message)
{    
    //show_message(message);
    if(message.client.ip == (int)QHostAddress("127.0.0.1").toIPv4Address()){
        emit on_subserver_api_message_received(message);
    }

    switch (message.type) {
    case TEXT_MESSAGE_TYPE:{
        //show_message(message);
        break;
    }
    case NO_CINFIRM_TEXT_MESSAGE_TYPE:{
        //show_message(message);
        break;
    }
    case CONFIRMATION_MESSAGE_TYPE:{
        for(int i = 0; i < waitingForConfirmation.length(); i++){
            if(((Message)waitingForConfirmation[i]).id == message.id){
                waitingForConfirmation.removeAt(i);
                break;
            }
        }
        break;
    }
    case REQUEST_ANSWER_MESSAGE_TYPE:{
        emit request_answer(message);
        break;
    }
    case ABSTRACT_REQUEST_MESSAGE_TYPE:{
        int answer = 42;
        send_message(Message(message.id, REQUEST_ANSWER_MESSAGE_TYPE, (SymbolType*)&answer, 4 / sizeof(SymbolType), message.client));
        break;
    }
        // Тут еще надо код со всякими emit...
    default:{
        throw new MessageProcessingException(String("unknown message type received"), UNKNOWN_MESSAGE_TYPE_EXCEPTION_ID);
    }
    }



    if(needConfirmation.contains(message.type)){
        //std::cout << "Need confirmation" << std::endl;
        Message confirmationMessage = Message();
        confirmationMessage.type = CONFIRMATION_MESSAGE_TYPE;
        confirmationMessage.id = message.id;
        confirmationMessage.data = (SymbolType*)"Message received!";
        confirmationMessage.size = 18;
        confirmationMessage.client = message.client;
        send_message(confirmationMessage);
    }
}

void MainServerNetworker::_resend_not_confirmed_messages()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeToResend));
        for(int i = 0; i < waitingForConfirmation.length(); i++){
            //std::cout << "resending..." << std::endl;
            String mes = String();
            try {
                mes = Crypto::wrap_message((Message)waitingForConfirmation[i]);
            } catch (Exception* exception) {
                exception->show();
                return;
            }
            _send_message(mes.data, mes.size, QHostAddress(((Message)waitingForConfirmation[i]).client.ip), ((Message)waitingForConfirmation[i]).client.port);
        }
    }
}

void MainServerNetworker::late_send_message(Message message, int delay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    send_message(message);
}

void MainServerNetworker::receive_message() {
    //std::cout << "Received some message" << std::endl;
    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());

    QHostAddress *address = new QHostAddress();
    quint16 port;
    auto s = socket->readDatagram(datagram.data(), datagram.size(), address, &port);

    if(s > 0){
        Message trueData;
        try {
            trueData = Crypto::parse_data(datagram.data(), datagram.size());
        } catch (Exception* exception) {
            exception->show();
            return;
        }

        trueData.client.ip = address->toIPv4Address();
        trueData.client.port = port;
        if(trueData.id > currentMaxId){
            currentMaxId = trueData.id;
        }

        _process_message(trueData);
    }

}

void MainServerNetworker::show_message(Message message)
{
    std::cout << "[on port " << myPort << "]\n";
    std::cout << "Received a message" << std::endl;
    std::cout << "Message : \n" << "    id : " << message.id << "\n    type : " << message.type << "\n    size : " << message.size << "\n    data : ";
    for(int i = 0; i < message.size; i++){
        std::cout << message.data[i];
    }
    std::cout << std::endl;
    //std::cout << datagram.data() << std::endl;
    unsigned char* ip_char = (unsigned char*)&message.client.ip;
    std::cout << "from ";
    for(int i = 0; i < 4; i++){
        std::cout << (int)ip_char[3-i];
        if(i < 3){
            std::cout << ".";
        }
    }

    std::cout << std::endl;
    std::cout << "port " << message.client.port << std::endl;
}
