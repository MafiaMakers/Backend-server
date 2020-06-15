#include "networkrequest.h"
#include "Exceptions/requestexception.h"
using namespace Mafia;
using namespace Requests;

NetworkRequest::NetworkRequest(MainServerNetworker *_networker, Message toAsk)
    : Request(), id(_networker->send_message(toAsk)), client(toAsk.client)
{
    data = 0;
    size = 0;
    //std::cout << _networker << " -- networker" << std::endl;
    connect(_networker, &MainServerNetworker::request_answer, this, &NetworkRequest::tryClose);
    ;
    std::cout << "Request initialized" << std::endl;
    std::cout << "Request data is: " << toAsk.data << std::endl;
}

void NetworkRequest::tryClose(Message received)
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
            throw new RequestException(System::String("Clients doesn't match!"), Exceptions::RequestExceptionId_WrongClient);
        }
    } else{
        std::cout << "Id not matching. Expected id" << id << ", but " << received.id << " is given\n";
    }
}



