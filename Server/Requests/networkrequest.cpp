#include "networkrequest.h"
#include "Exceptions/requestexception.h"
using namespace Mafia;
using namespace Requests;

NetworkRequest::NetworkRequest(Network::MainServerNetworker *_networker, Network::Message toAsk)
    : Request(), id(_networker->send_message(toAsk)), client(toAsk.client)
{
    data = 0;
    size = 0;
    //std::cout << _networker << " -- networker" << std::endl;
    connect(_networker, &Network::MainServerNetworker::request_answer, this, &NetworkRequest::tryClose);
    ;
    std::cout << "Request initialized" << std::endl;
	std::cout << "Request data is: " << toAsk.data << std::endl;
}

NetworkRequest::~NetworkRequest()
{
	SAFE_DELETE_ARRAY(this->data);
}

void NetworkRequest::tryClose(Network::Message received)
{
	//Ответом на запрос должно быть сообщение с таким же id
    if(received.id == id){
		//Кроме того во избежание ошибок проверим клиента-отправителя
        if(received.client == client){
			//Копируем данные из сообщения
			SAFE_NEW(this->data, Network::SymbolType[received.size]);
            for(int i = 0; i < received.size; i++){
                this->data[i] = received.data[i];
            }
            this->size = received.size;

            close_request();
        } else{
			throw Exceptions::Exception::generate(System::String("Clients doesn't match!"), Exceptions::RequestExceptionId_WrongClient);
        }
	} /*else{
        std::cout << "Id not matching. Expected id" << id << ", but " << received.id << " is given\n";
	}*/
}



