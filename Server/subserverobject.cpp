#include "subserverobject.h"
#include "System/runprogram.h"
#include "Exceptions/subserverexception.h"
#include <thread>
using namespace Mafia;

SubserverObject::SubserverObject(QObject *parent) : QObject(parent){}

SubserverObject::SubserverObject(MainServerNetworker* networker, int port, const String _path, const String _processName,
                                 int checkInterval, int maxNotAnswering, const String specialCommands)
    : path(_path), processName(_processName), checkRespondInterval(checkInterval), maxNotAnsweringsCount(maxNotAnswering)
{
    this->networker = networker;

    this->myAddress.ip = QHostAddress("127.0.0.1").toIPv4Address();

    this->myAddress.port = port;

    this->pid = -1;

    this->currentRequests = QList<RequestWithId>();

    connect(this, &SubserverObject::on_crash, this, &SubserverObject::crash_processing);

    connect(networker, &MainServerNetworker::on_subserver_api_message_received, this, &SubserverObject::message_from_server);

    std::thread runThread(&SubserverObject::run_me, this, specialCommands);
    runThread.detach();

}

int SubserverObject::send_request(MessageTypeType type, SymbolType *data, int size)
{
    NetworkRequest* req = 0;
    try {
        req = new NetworkRequest(this->networker, Message(type, data, size, this->myAddress));
    } catch (Exception* exception) {
        switch (exception->get_id()) {
        default:{
            exception->show();
            return -2;
        }
        }
    }

    int myId = 0;
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(this->currentRequests[i].id > myId){
            myId = this->currentRequests[i].id;
        }
    }
    myId++;

    RequestWithId current = RequestWithId();
    current.id = myId;
    current.request = req;

    connect(req, &NetworkRequest::on_ready_me, this, &SubserverObject::request_answered);

    this->currentRequests.append(current);

    return myId;
}

void SubserverObject::pass_client(Client client)
{
    try {
        this->send_message_to_subserver(PASS_CLIENT_MESSAGE_TYPE, (SymbolType*)&client, sizeof (client) / sizeof(SymbolType));
    } catch (Exception* exception) {
        switch(exception->get_id()){
        default:{
            exception->show();
            return;
        }
        }
    }
}

bool SubserverObject::is_request_ready(RequestIdType requestId)
{
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(requestId == currentRequests[i].id){
            return currentRequests[i].request->is_finished();
        }
    }
    throw new SubserverException(String("request doesn't in requests list"), NO_SUCH_REQUEST_EXCEPTION_ID);
    return false;
}

int SubserverObject::send_message_to_subserver(MessageTypeType type, SymbolType *data, int size, MessageIdType id)
{
    Message message = Message(type, data, size, myAddress, id);
    try {
        return networker->send_message(message);
    } catch (Exception* exception) {
        switch(exception->get_id()){
        default:{
            exception->show();
            return -2;
        }
        }
    }

}

void SubserverObject::request_answered(Request *request)
{
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(request == currentRequests[i].request){
            emit on_request_ready(currentRequests[i].id);
            return;
        }
    }
    throw new SubserverException(String("request doesn't in requests list"), NO_SUCH_REQUEST_EXCEPTION_ID);
}

template<class T>
T SubserverObject::get_request_result(RequestIdType requestId)
{
    for(int i = 0; i < this->currentRequests.length(); i++){
        if(currentRequests[i].id == requestId){
            try {
                return currentRequests[i].request->get_result<T>();
            } catch (Exception* exception) {
                switch(exception->get_id()){
                default:{
                    throw exception;
                }
                }
            }
        }
    }
    throw new SubserverException(String("request doesn't in requests list"), NO_SUCH_REQUEST_EXCEPTION_ID);
}

void SubserverObject::crash_processing()
{
    if(this->pid != -1){
        kill(this->pid);
    }
    run_me(String("after crash"));
}

void SubserverObject::_check_subserver_respond()
{
    while(this->initialized){
        if(notAnsweringsCount >= maxNotAnsweringsCount){
            emit on_crash();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(this->checkRespondInterval));
        try {
            networker->send_message(Message(CHECK_CONNECTION_MESSAGE_TYPE, (SymbolType*)(char*)"check", 6 / sizeof(SymbolType), myAddress));
        } catch (Exception* exception) {
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

void SubserverObject::run_me(const String specialCommands)
{
    try {
        this->pid = run_app(this->path, this->processName, specialCommands);
    } catch (Exception* exception) {
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

void SubserverObject::message_from_server(Message message)
{
    if(message.client == this->myAddress){
        switch (message.type) {
        case CHECK_CONNECTION_MESSAGE_TYPE:{
            notAnsweringsCount = 0;
            break;
        }
        default:{
            emit on_message_got(message);
        }
        }

    }
}
