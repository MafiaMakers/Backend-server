#include <iostream>
#include <thread>

#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include "messageTypes.h"
#include "Exceptions/messageprocessingexception.h"
#include <System/serializer.h>

using namespace Mafia;
using namespace Network;

const int MainServerNetworker::TIME_TO_RESEND = 2000;
const int MainServerNetworker::MAX_RESEND_COUNT = 100;
//const int MainServerNetworker::idsForClient = 1000;

const MafiaList<MessageType> MainServerNetworker::needConfirmation = MafiaList<MessageType>()
		/*<< MessageType_Text
        << MessageType_RequestAnswer
        << MessageType_AbstractRequest
		<< MessageType_PassClientRequest*/;

MainServerNetworker::MainServerNetworker(QObject* parent) : QObject(parent){}

MainServerNetworker::MainServerNetworker(int port)
{
	//Crypto::set_key("TEST_key_thisIs");

    this->waitingToFillMessages = MafiaList<MafiaList<Message>>();
    this->currentMaxId = 0;

#ifndef DONT_USE_QT
	this->sender = new MESSAGE_SENDER(port);
	connect(sender, &MESSAGE_SENDER::message_received, this, &MainServerNetworker::receive_message);
#endif

    this->waitingForConfirmation = MafiaList<Message>();
    std::thread resendingThread(&MainServerNetworker::_resend_not_confirmed_messages, this);
    resendingThread.detach();

	std::cout << "Initialized server on port " << port << std::endl;
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

void MainServerNetworker::_send_message(char* data, int size, int client, int port) {
	sender->send(data, size, client, port);
}

void MainServerNetworker::_process_message(Message message)
{
	std::cerr << "processing func\n";
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
	if(message.client.ip == LOCALHOST_IP){
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
		//std::cerr << "received_message called\n";
		//В любой непонятной ситуации передавай управление основному менеджеру
        emit message_received(message);


		//throw Exceptions::Exception::generate(System::String("unknown message type received"),
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
		throw Exceptions::Exception::generate(System::String("No such message id in list"),
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
	//std::cerr << "adding received message\n";
    int listIndex = get_list_index(message.id);
	//std::cerr << "list index: " << listIndex << "\n";
	// Если такого сообщения еще нет в списке ожидающих, значит это первая часть от сообщения, которая нам пришла
    if(listIndex == -1){
		//std::cerr << "INDEX IS -1!!\n";
        listIndex = waitingToFillMessages.length();
		//std::cout << "Addind empty\n";
        _add_empty_message(message);
		//std::cerr << listIndex << " - new list index\n";
    }
    try {
		//Если в списке не произошло какой-либо ошибки
		if( message.partIndex < waitingToFillMessages[ listIndex ].length() - 1 && message_matches( message ) ){
			//std::cerr << "all ok\n";
			waitingToFillMessages[ listIndex ][ message.partIndex + 1 ] = message;
        } else{
			throw Exceptions::Exception::generate(System::String("Message parts data mismatch"),
                                                             Exceptions::MessageProcessingExceptionId_MessagePartsMismatch);
        }

		//std::cerr << "checking full message\n";
		//Если сообщение получено целиком, то собираем его и отправляем на обработку
        if(check_message_full(message.id)){
			//std::cerr << "constructing result\n";
            Message wholeMessage = _construct_whole_message(message.id);

            waitingToFillMessages.removeAt(listIndex);
			//std::cerr << "processing!!!\n";
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
		throw Exceptions::Exception::generate(System::String("No such message id in list"),
                                                         Exceptions::MessageProcessingExceptionId_MissingMessageId);
        return false;
    }

    if(message.type != waitingToFillMessages[index][0].type){
		//std::cerr << "TYPE MISMATCH\n";
        return false;
    }

    if(message.client != waitingToFillMessages[index][0].client){
		//std::cerr << "CLIENT MISMATCH\n";
        return false;
    }

    if(message.partsCount != waitingToFillMessages[index][0].partsCount){
		//std::cerr << "PARTS COUNT MISMATCH\n";
        return false;
    }
	//std::cout << "messages math\n";
    return true;
}

void MainServerNetworker::receive_message(char* data, int size, int ip, int port) {
	//std::cerr << "--Message received!\n";


	if(size > 0){
		//Дешифруем сообщение
        Message trueData;
        try {
			trueData = Crypto::parse_data(data, size);
			//std::cerr << "decoding succeed\n";
        } catch (Exceptions::Exception* exception) {
            exception->show();
            return;
        }

		//Заполняем его поля клиента данными
		trueData.client.ip = ip;
        trueData.client.port = port;
        if(trueData.id > currentMaxId){
            currentMaxId = trueData.id;
        }
		//std::cerr << "data completed\n";
        //show_message(trueData);

        add_received_message(trueData);
    }

}

void MainServerNetworker::show_message(Message message)
{
	//std::cout << "[on port " << myPort << "]";
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
		_send_message(mes.data, mes.size, message.client.ip, message.client.port);

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
		throw Exceptions::Exception::generate(
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

#ifdef DONT_USE_QT
#include "defnetworkersignals.h"
#endif
