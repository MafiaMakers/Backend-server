#include "networkrequest.h"
#include "Exceptions/requestexception.h"
Mafia::NetworkRequest::NetworkRequest(Mafia::MainServerNetworker *_networker, Mafia::Message toAsk)
    : Request(), id(toAsk.id), client(toAsk.client)
{
    data = 0;
    size = 0;
    //std::cout << _networker << " -- networker" << std::endl;
    connect(_networker, &MainServerNetworker::request_answer, this, &NetworkRequest::tryClose);
    _networker->send_message(toAsk);
    std::cout << "Request initialized" << std::endl;
    std::cout << "Request data is: " << toAsk.data << std::endl;
}

void Mafia::NetworkRequest::tryClose(Mafia::Message received)
{
    if(received.id == id){
        if(received.client == client){
            close_request();
            this->data = new SymbolType[received.size];
            for(int i = 0; i < received.size; i++){
                this->data[i] = received.data[i];
            }
            this->size = received.size;

            close_request();
        } else{
            throw new RequestException(String("Clients doesn't match!"), WRONG_CLIENT_EXCEPTION_ID);
        }
    }
}



