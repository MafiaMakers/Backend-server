#include <iostream>
#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include <QUdpSocket>
#include <QtCore>
#include <QSet>
#include "messageTypes.h"
#include "Exceptions/messageprocessingexception.h"
#include "System/functions.h"

using namespace Mafia;
using namespace Network;
const int MainServerNetworker::TIME_TO_RESEND = 500;
const int MainServerNetworker::MAX_RESEND_COUNT = 100;
//const int MainServerNetworker::idsForClient = 1000;

const QSet<MessageTypeType> MainServerNetworker::needConfirmation = QSet<MessageTypeType>()
        << MessageType_Text
        << MessageType_RequestAnswer
        << MessageType_AbstractRequest
        << MessageType_PassClientRequest;

MainServerNetworker::MainServerNetworker(int port)
{
    this->waitingToFillMessages = MafiaList<MafiaList<Message>>();
    this->currentMaxId = 0;
    this->socket = new QUdpSocket(this);
    this->myPort = port;
    this->socket->bind(QHostAddress::Any, this->myPort);
    this->waitingForConfirmation = MafiaList<Message>();
    std::thread resendingThread(&MainServerNetworker::_resend_not_confirmed_messages, this);
    resendingThread.detach();
    connect(socket, SIGNAL(readyRead()), this, SLOT(receive_message()));
}

MessageIdType MainServerNetworker::send_message(Message message)
{
    System::String mes = System::String();
    if(message.id == (MessageIdType)(0)){
        currentMaxId++;
        message.id = currentMaxId;
    }

    int standartSize = (MAX_MESSAGE_SIZE / sizeof(SymbolType));

    message.partsCount = (message.size / standartSize) + 1;



    for(int i = 0; i < message.partsCount; i++){
        Message partMes = message;
        partMes.partIndex = i;
        partMes.data = &(message.data[i * standartSize]);
        if(i == message.partsCount - 1){
            partMes.size = message.size - (message.partsCount - 1) * standartSize;
        } else{
            partMes.size = standartSize;
        }
        try {
            mes = Crypto::wrap_message(partMes);
        } catch (Exceptions::Exception* exception) {
            exception->show();
            return -2;
        }
        std::cout << "Sent message : " << std::string(partMes.data, partMes.size) << std::endl;
        _send_message(mes.data, mes.size, QHostAddress(message.client.ip), message.client.port);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if(needConfirmation.contains(message.type)){
        waitingForConfirmation.append(message);
    }

    return message.id;
    //std::cout << "Sent a message" << std::endl;
}

void MainServerNetworker::_send_message(char* data, int size, QHostAddress client, int port) {
    //std::cout << "Sent " << data << " of size " << size << " to client with ip " << client.toString().toStdString() << " to port " << port << std::endl;
    socket->writeDatagram(data, size, client, port);
}

void MainServerNetworker::_process_message(Message message)
{    

    if(needConfirmation.contains(message.type)){
        //std::cout << "Need confirmation" << std::endl;
        Message confirmationMessage = Message();
        confirmationMessage.type = MessageType_Confirmation;
        confirmationMessage.id = message.id;
        confirmationMessage.data = (SymbolType*)"Message received!";
        confirmationMessage.size = 18;
        confirmationMessage.client = message.client;
        send_message(confirmationMessage);
    }
    //show_message(message);
    if(message.client.ip == (int)QHostAddress("127.0.0.1").toIPv4Address()){
        emit on_subserver_api_message_received(message);
        return;
    }

    switch (message.type) {
    case MessageType_Text:{
        show_message(message);
        break;
    }
    case MessageType_NoConfirmText:{
        show_message(message);
        break;
    }
    case MessageType_Confirmation:{
        for(int i = 0; i < waitingForConfirmation.length(); i++){
            if(((Message)waitingForConfirmation[i]).id == message.id){
                waitingForConfirmation.removeAt(i);
                break;
            }
        }
        break;
    }
    case MessageType_RequestAnswer:{
        emit request_answer(message);
        break;
    }
    case MessageType_AbstractRequest:{
        int answer = 42;
        send_message(Message(MessageType_RequestAnswer, (SymbolType*)&answer, 4 / sizeof(SymbolType), message.client, message.id));
        break;
    }
        // Тут еще надо код со всякими emit...
    default:{
        emit message_received(message);
        //throw new Exceptions::MessageProcessingException(System::String("unknown message type received"), Exceptions::MessageProcessingExceptionId_UnknownMessageType);
    }
    }
}

void MainServerNetworker::_resend_not_confirmed_messages()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_TO_RESEND));
        for(int i = 0; i < waitingForConfirmation.length(); i++){
            //std::cout << "resending..." << std::endl;
            System::String mes = System::String();
            try {
                mes = Crypto::wrap_message((Message)waitingForConfirmation[i]);
            } catch (Exceptions::Exception* exception) {
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

bool MainServerNetworker::check_message_full(MessageIdType id)
{
    int listIndex = get_list_index(id);

    if(listIndex == -1){
        throw new Exceptions::MessageProcessingException(System::String("No such exception id in list"),
                                                         Exceptions::MessageProcessingExceptionId_MissingMessageId);
        return false;
    }

    for(int i = 0; i < waitingToFillMessages[listIndex].length(); i++){
        if(waitingToFillMessages[listIndex][i].id != id){
            return false;
        }
    }
    return true;
}

int MainServerNetworker::get_list_index(MessageIdType id)
{
    for(int i = 0; i < waitingToFillMessages.length(); i++){
        if(waitingToFillMessages[i][0].id == id){
            return i;
        }
    }
    return  -1;
}

void MainServerNetworker::add_received_message(Message message)
{
    int listIndex = get_list_index(message.id);

    if(listIndex == -1){
        listIndex = waitingToFillMessages.length();
        waitingToFillMessages.append(MafiaList<Message>());

        Message base = Message();
        base.id = message.id;
        base.type = message.type;
        base.client = message.client;
        base.partsCount = message.partsCount;

        waitingToFillMessages[listIndex].append(base);

        for(int i = 0; i < message.partsCount; i++){
            waitingToFillMessages[listIndex].append(Message());
        }
    }
    try {
        if(message.partIndex < waitingToFillMessages[listIndex].length() - 1 && message_matches(message)){
            waitingToFillMessages[listIndex][message.partIndex + 1] = message;
        } else{
            throw new Exceptions::MessageProcessingException(System::String("Message parts data mismatch"),
                                                             Exceptions::MessageProcessingExceptionId_MessagePartsMismatch);
        }

        if(check_message_full(message.id)){
            Message wholeMessage = Message();
            wholeMessage.id = message.id;
            wholeMessage.type = message.type;
            wholeMessage.client = message.client;
            wholeMessage.partsCount = 1;
            wholeMessage.partIndex = 0;
            wholeMessage.size = 0;

            for(int i = 1; i < waitingToFillMessages[listIndex].length(); i++){
                wholeMessage.size += waitingToFillMessages[listIndex][i].size;
            }

            wholeMessage.data = new SymbolType[wholeMessage.size];
            int currentInd = 0;
            for(int i = 1; i < waitingToFillMessages[listIndex].length(); i++){
                for(int j = 0; j < waitingToFillMessages[listIndex][i].size; j++){
                    wholeMessage.data[currentInd] = waitingToFillMessages[listIndex][i].data[j];
                    currentInd++;
                }
            }

            waitingToFillMessages.removeAt(listIndex);

            _process_message(wholeMessage);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            exception->show();
        }
        }
    }
}

bool MainServerNetworker::message_matches(Message message)
{
    int index = get_list_index(message.id);

    if(index == -1){
        throw new Exceptions::MessageProcessingException(System::String("No such message id in list"),
                                                         Exceptions::MessageProcessingExceptionId_MissingMessageId);
        return false;
    }

    if(message.type != waitingToFillMessages[index][0].type){
        return false;
    }

    if(message.client != waitingToFillMessages[index][0].client){
        return false;
    }

    if(message.partsCount != waitingToFillMessages[index][0].partsCount){
        return false;
    }

    return true;
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
        } catch (Exceptions::Exception* exception) {
            exception->show();
            return;
        }

        trueData.client.ip = address->toIPv4Address();
        trueData.client.port = port;
        if(trueData.id > currentMaxId){
            currentMaxId = trueData.id;
        }
        //show_message(trueData);
        add_received_message(trueData);
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
