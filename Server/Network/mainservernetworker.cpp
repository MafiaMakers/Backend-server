#include <iostream>
#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include <QUdpSocket>
#include <QtCore>
#include <QSet>
#include "messageTypes.h"
#include "Exceptions/messageprocessingexception.h"
#include "System/functions.h"
#include <System/serializer.h>

using namespace Mafia;
using namespace Network;
const int MainServerNetworker::TIME_TO_RESEND = 2000;
const int MainServerNetworker::MAX_RESEND_COUNT = 100;
//const int MainServerNetworker::idsForClient = 1000;

const QSet<MessageType> MainServerNetworker::needConfirmation = QSet<MessageType>()
        << MessageType_Text
        << MessageType_RequestAnswer
        << MessageType_AbstractRequest
        << MessageType_PassClientRequest;

MainServerNetworker::MainServerNetworker(QObject* parent) : QObject(parent){}

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
	connect(socket, &QUdpSocket::readyRead, this, &MainServerNetworker::receive_message);
}

MainServerNetworker::~MainServerNetworker()
{

}

MessageIdType MainServerNetworker::send_message(Message message)
{
	//std::cout << "sending message : \n";
	//show_message(message);

	//Находим допустимый id, чтобы он не пересекался с другими id. (Если у сообщения был установлен id, то мы его оставляем)
    if(message.id == (MessageIdType)(0)){
        currentMaxId++;
        message.id = currentMaxId;
    }

	//Отправляем сообщение
	_resend_message(message);

    if(needConfirmation.contains(message.type)){
        waitingForConfirmation.append(message);
    }

    return message.id;
    //std::cout << "Sent a message" << std::endl;
}

void MainServerNetworker::_send_message(char* data, int size, QHostAddress client, int port) {
	/*std::cout << "Sent " << data
	 * << " of size " << size
	 * << " to client with ip " << client.toString().toStdString()
	 * << " to port " << port << std::endl;*/
	socket->writeDatagram(data, size, client, port);
}

void MainServerNetworker::_process_message(Message message)
{
	//Если это сообщение требует подтверждения, то надо отправить сообщение с подтвреждением
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

	//Если это сообщение субсервера, то объекты субсервера должны проверить:
	//вдруг это системное сообщение, а не сообщение действия
	if(message.client.ip == (int)QHostAddress("127.0.0.1").toIPv4Address()){
        emit on_subserver_api_message_received(message);
		//return;
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
        _confirm_message(message.id);
        break;
    }
    case MessageType_RequestAnswer:{
        emit request_answer(message);
        break;
    }
	//Ну это просто тестовая штуковина)) Просто обработка какого-то запроса
    case MessageType_AbstractRequest:{
		int answer = 42;
		System::String mesData = System::Serializer::serialize<int>(answer);
		send_message(Message(
						 MessageType_RequestAnswer,
						 (SymbolType*)mesData.data,
						 mesData.size / sizeof(SymbolType),
						 message.client,
						 message.id)
					 );
        break;
    }
    default:{
		//В любой непонятной ситуации передавай управление основному менеджеру
        emit message_received(message);


		//throw new Exceptions::MessageProcessingException(System::String("unknown message type received"),
			//Exceptions::MessageProcessingExceptionId_UnknownMessageType);
    }
    }
}

void MainServerNetworker::_resend_not_confirmed_messages()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_TO_RESEND));

		//Проходимся по всем неотправленным сообщениям и заново их отправляем.
        for(int i = 0; i < waitingForConfirmation.length(); i++){
			_resend_message( waitingForConfirmation[ i ] );
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

	//Если такое сообщение не найдено, кидаем ошибку
    if(listIndex == -1){
		throw new Exceptions::MessageProcessingException(System::String("No such message id in list"),
                                                         Exceptions::MessageProcessingExceptionId_MissingMessageId);
        return false;
    }

	//Проходимся по всем частям и проверяем, что их id совпадает
    for(int i = 0; i < waitingToFillMessages[listIndex].length(); i++){
		//Если часть сообщения еще не получена, вместо нее будет стоять пустое сообщение с id 0, а значит, не совпадет с id базы
        if(waitingToFillMessages[listIndex][i].id != id){
            return false;
        }
    }
    return true;
}

int MainServerNetworker::get_list_index(MessageIdType id)
{
    for(int i = 0; i < waitingToFillMessages.length(); i++){
		if(waitingToFillMessages[i].length() > 1){
			if(waitingToFillMessages[i][0].id == id){
				return i;
			}
		}
    }
    return  -1;
}

void MainServerNetworker::add_received_message(Message message)
{
    int listIndex = get_list_index(message.id);
	// Если такого сообщения еще нет в списке ожидающих, значит это первая часть от сообщения, которая нам пришла
    if(listIndex == -1){
        listIndex = waitingToFillMessages.length();
        _add_empty_message(message);
    }
    try {
		//Если в списке не произошло какой-либо ошибки
		if( message.partIndex < waitingToFillMessages[ listIndex ].length() - 1 && message_matches( message ) ){
			waitingToFillMessages[ listIndex ][ message.partIndex + 1 ] = message;
        } else{
            throw new Exceptions::MessageProcessingException(System::String("Message parts data mismatch"),
                                                             Exceptions::MessageProcessingExceptionId_MessagePartsMismatch);
        }

		//Если сообщение получено целиком, то собираем его и отправляем на обработку
        if(check_message_full(message.id)){
            Message wholeMessage = _construct_whole_message(message.id);

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
	//std::cout << "check message match\n";
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
	//std::cout << "messages math\n";
    return true;
}

void MainServerNetworker::receive_message() {
	//std::cout << "Message received!\n";

	//Получаем сообщение из сокета
    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());

    QHostAddress *address = new QHostAddress();
    quint16 port;
	auto bytesReceived = socket->readDatagram(datagram.data(), datagram.size(), address, &port);

	if(bytesReceived > 0){
		//Дешифруем сообщение
        Message trueData;
        try {
            trueData = Crypto::parse_data(datagram.data(), datagram.size());
        } catch (Exceptions::Exception* exception) {
            exception->show();
            return;
        }

		//Заполняем его поля клиента данными
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
	std::cout << "[on port " << myPort << "]";
	std::cout << "\nMessage : "
			  << "\n    id : " << message.id
			  << "\n    type : " << message.type
			  << "\n    size : " << message.size
			  << "\n    data : ";
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

void MainServerNetworker::_resend_message(Message message)
{
	System::String mes = System::String();

	//Считаем размер сообщения, по которому будем их дробить. И, соответственно, количество частей сообщения
	int standartSize = (MAX_MESSAGE_SIZE / sizeof(SymbolType));
	message.partsCount = (message.size / standartSize) + 1;

	for(int i = 0; i < message.partsCount; i++){
		//Создаем новое сообщение, которое будет частью большого
		Message partMes = message;
		partMes.partIndex = i;
		partMes.data = &(message.data[i * standartSize]);
		//Если это последний кусок, то его размер может быть несколько меньше стандартного
		if(i == message.partsCount - 1){
			partMes.size = message.size - (message.partsCount - 1) * standartSize;
		} else{
			partMes.size = standartSize;
		}
		try {
			//Шифруем сообщение в строку
			mes = Crypto::wrap_message(partMes);
		} catch (Exceptions::Exception* exception) {
			exception->show();
			return;
		}
		//std::cout << "Sent message : " << std::string(partMes.data, partMes.size) << std::endl;
		//Отправляем
		_send_message(mes.data, mes.size, QHostAddress(message.client.ip), message.client.port);

		/*А вот это какой-то кринжец, без которого прога почему-то не работает...
		 *Если не ждать 1 мс, то происходят беды... ПАМАГИТИ
		 *Сообщения просто не отправляются. Возможно, что-то связано с тем, что они не успевают отправиться, как уже следующее на подходе
		*/
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void MainServerNetworker::_add_empty_message(Message baseMessage)
{
	//Т.к. мы добавляем новый элемент, его индекс равен текущей длине списка
    int listIndex = waitingToFillMessages.length();

	/*Нулевой элемент этого списка - это базовое сообщение.
	 *Оно пустое и лишь отвечает за то, какой id и сколько частей у этого сообщения есть
	 *После базового сообщения идут уже сами части сообщения. Изначально они заполнены пустышками*/
    Message base = Message();
    base.id = baseMessage.id;
    base.type = baseMessage.type;
    base.client = baseMessage.client;
    base.partsCount = baseMessage.partsCount;

	waitingToFillMessages.append(MafiaList<Message>());

    waitingToFillMessages[listIndex].append(base);

    for(int i = 0; i < baseMessage.partsCount; i++){
        waitingToFillMessages[listIndex].append(Message());
    }
}

Message MainServerNetworker::_construct_whole_message(MessageIdType id)
{
    int listIndex = get_list_index(id);

	//Если не нашли такой элемент, то кидаем исключение
	if(listIndex == -1){
		throw new Exceptions::MessageProcessingException(
					System::String("No such message id in list"),
					Exceptions::MessageProcessingExceptionId_MissingMessageId);
	}

	//Создаем целое сообщение и заполняем его общие параметры
    Message wholeMessage = Message();
    wholeMessage.id = waitingToFillMessages[listIndex][0].id;
    wholeMessage.type = waitingToFillMessages[listIndex][0].type;
    wholeMessage.client = waitingToFillMessages[listIndex][0].client;
    wholeMessage.partsCount = 1;
    wholeMessage.partIndex = 0;
    wholeMessage.size = 0;

	//Для начала вычислим размер итогового сообщения
    for(int i = 1; i < waitingToFillMessages[listIndex].length(); i++){
        wholeMessage.size += waitingToFillMessages[listIndex][i].size;
    }

	//А затем уже собираем по частям сами данные
    wholeMessage.data = new SymbolType[wholeMessage.size];
    int currentInd = 0;
    for(int i = 1; i < waitingToFillMessages[listIndex].length(); i++){
        for(int j = 0; j < waitingToFillMessages[listIndex][i].size; j++){
            wholeMessage.data[currentInd] = waitingToFillMessages[listIndex][i].data[j];
            currentInd++;
        }
    }
    return wholeMessage;
}

void MainServerNetworker::_confirm_message(MessageIdType id)
{
	//Находим в списке сообщение и удаляем его
    for(int i = 0; i < waitingForConfirmation.length(); i++){
        if(((Message)waitingForConfirmation[i]).id == id){
            waitingForConfirmation.removeAt(i);
            break;
        }
    }
}
