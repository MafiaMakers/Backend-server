#include "subserverobject.h"
#include "System/runprogram.h"
#include "Exceptions/subserverexception.h"
#include "System/portsmanager.h"
#include <thread>
using namespace Mafia;
using namespace Subservers;

SubserverObject::SubserverObject(QObject *parent) : QObject(parent){}

SubserverObject::SubserverObject(Network::MainServerNetworker* networker, int port, const System::String _path, const System::String _processName,
                                 int checkInterval, int maxNotAnswering, const System::String specialCommands)
    : path(_path), processName(_processName), checkRespondInterval(checkInterval), maxNotAnsweringsCount(maxNotAnswering)
{
    this->networker = networker;

    this->myAddress.ip = QHostAddress("127.0.0.1").toIPv4Address();

    this->myAddress.port = port;

    this->pid = -1;

    this->currentRequests = MafiaList<RequestWithId>();

    connect(this, &SubserverObject::on_crash, this, &SubserverObject::crash_processing);

    connect(networker, &Network::MainServerNetworker::on_subserver_api_message_received, this, &SubserverObject::message_from_server);

    std::thread runThread(&SubserverObject::run_me, this, specialCommands);
    runThread.detach();

}

SubserverObject::~SubserverObject()
{

}

int SubserverObject::send_request(Network::MessageType type, Network::SymbolType *data, int size)
{
	//Создаем новый запрос
    Requests::NetworkRequest* req = 0;
    try {
		SAFE_NEW(req, Requests::NetworkRequest(this->networker, Network::Message(type, data, size, this->myAddress)));
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            exception->show();
            return -2;
        }
        }
    }

	//Находим максимальный id из списка
    int myId = 0;
    for(int i = 0; i < this->currentRequests.length(); i++){
		if(this->currentRequests[i].id > myId){
            myId = this->currentRequests[i].id;
        }
    }
	//И увеличиваем наш на 1
    myId++;

	//Собираем запрос с id
    RequestWithId current = RequestWithId();
    current.id = myId;
    current.request = req;

	//Подключаемся к сигналу готовности
    connect(req, &Requests::NetworkRequest::on_ready_me, this, &SubserverObject::request_answered);

    this->currentRequests.append(current);

    return myId;
}

bool SubserverObject::is_request_ready(RequestIdType requestId)
{
	//Находим в списке запрос по id
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(requestId == currentRequests[i].id){
            return currentRequests[i].request->is_finished();
        }
    }
	throw Exceptions::Exception::generate(System::String("request is not in requests list"), Exceptions::SubserverExceptionId_NoSuchRequest);
    return false;
}

int SubserverObject::send_message_to_subserver(Network::MessageType type, Network::SymbolType *data, int size, Network::MessageIdType id)
{
    Network::Message message = Network::Message(type, data, size, myAddress, id);
    try {
        return networker->send_message(message);
    } catch (Exceptions::Exception* exception) {
        switch(exception->get_id()){
        default:{
            exception->show();
            return -2;
        }
        }
    }

}

Network::Client SubserverObject::get_my_address()
{
    return myAddress;
}

void SubserverObject::finish_work()
{
    System::kill(this->pid);
    System::PortsManager::free_port(myAddress.port);
}

void SubserverObject::request_answered(Requests::Request *request)
{
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(request == currentRequests[i].request){
            emit on_request_ready(currentRequests[i].id);
            return;
        }
    }
	//Вот я не уверен, что стоит тут кидать исключение... Это ведь слот
	//throw Exceptions::Exception::generate(
	//		System::String("request doesn't in requests list"), Exceptions::SubserverExceptionId_NoSuchRequest);
}

template<class T>
T SubserverObject::get_request_result(RequestIdType requestId)
{
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(currentRequests[i].id == requestId){
            return currentRequests[i].request->get_result<T>();
        }
    }
	throw Exceptions::Exception::generate(System::String("request doesn't in requests list"), Exceptions::SubserverExceptionId_NoSuchRequest);
}

void SubserverObject::crash_processing()
{
	//Просто перезапуск
    if(this->pid != -1){
        System::kill(this->pid);
    }
    run_me(System::String("after crash"));
}

void SubserverObject::_check_subserver_respond()
{
    while(this->initialized){
		//Если все уже плохо, то перезапустим
        if(notAnsweringsCount >= maxNotAnsweringsCount){
            emit on_crash();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(this->checkRespondInterval));
		//Отправляем субсерверу сообщение с запросом на подтверждение того, что у него все ок
        try {

			networker->send_message(Network::Message(
										Network::MessageType_CheckConnection,
										(Network::SymbolType*)(char*)"check",
										6 / sizeof(Network::SymbolType),
										myAddress)
									);

        } catch (Exceptions::Exception* exception) {
            switch (exception->get_id()) {
            default:{
                exception->show();
                continue;
            }
            }
        }
        notAnsweringsCount++;
    }
}

void SubserverObject::run_me(const System::String specialCommands)
{
	if(this->processName.size != 0){
		try {
			this->pid = run_app(this->path, this->processName, specialCommands);
		} catch (Exceptions::Exception* exception) {
			switch(exception->get_id()){
			default:{
				emit on_crash();
				exception->show();
				return;
			}
			}
		}
		this->initialized = true;

		std::thread checkThread(&SubserverObject::_check_subserver_respond, this);
		checkThread.detach();
	}
}

void SubserverObject::message_from_server(Network::Message message)
{
	//Этот метод будет переопределяться в наследниках.
	//Ну пока просто отмечаем, если субсервер ответил на проверку отклика
    if(message.client == this->myAddress){
        switch (message.type) {
        case Network::MessageType_CheckConnection:{
            notAnsweringsCount = 0;
            break;
        }
        default:{
            emit on_message_got(message);
        }
        }

    }
}
