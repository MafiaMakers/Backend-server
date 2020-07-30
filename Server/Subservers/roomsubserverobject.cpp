#include "roomsubserverobject.h"
#include "System/serializer.h"
#include "System/keygen.h"


using namespace Mafia;
using namespace Subservers;

const System::String RoomSubserverObject::exePath = System::String("");
const System::String RoomSubserverObject::exeProcessName = System::String("");

RoomSubserverObject::RoomSubserverObject(QObject *parent) : SubserverObject(parent){}

RoomSubserverObject::RoomSubserverObject(Network::MainServerNetworker *networker,
										 int port,
										 int checkInterval,
										 int maxNotAnswering,
										 const System::String specialCommands) :
	SubserverObject(networker,
					port,
					exePath,
					exeProcessName,
					checkInterval,
					maxNotAnswering,
					specialCommands),
	//Случайно генерируем код доступа к комнате
    connection_key(System::KeyGen::generate_key<QString>(KEY_SIZE)){}

RoomSubserverObject::~RoomSubserverObject(){}

void RoomSubserverObject::pass_client(Network::Client client, UserStatistics info)
{
    try {
		//Передаем все данные о клиенте субсерверу
        ClientInfo allInfo = ClientInfo();
        allInfo.client = client;
        allInfo.statistics = info;
        std::string data = System::Serializer::serialize<ClientInfo>(allInfo);

		networker->send_message(Network::Message(
									Network::MessageType_PassClientRequest,
									(Network::SymbolType*)data.c_str(),
									data.length(),
									myAddress)
								);

        myClients.append(allInfo);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
        }
        }
    }
}

MafiaList<ClientInfo> RoomSubserverObject::get_connected_clients()
{
    return myClients;
}

bool RoomSubserverObject::check_connection_key(QString key)
{
    return key == this->connection_key;
}

QString RoomSubserverObject::get_key()
{
    return connection_key;
}

void RoomSubserverObject::message_from_server(Network::Message message)
{
    if(message.client == this->myAddress){
        switch (message.type) {
        case Network::MessageType_CheckConnection:{
            notAnsweringsCount = 0;
            break;
        }
        case Network::MessageType_PlayerLeaveGame:{
			//Узнаем, какой именно клиент покинул игру
            System::String messageData = System::String(message.data, message.size);
            ClientInfo data = System::Serializer::deserialize<ClientInfo>(messageData);

			//Если такой действительно был, то удаляем его и сообщаем менеджеру
            if(myClients.contains(data)){
                emit client_leave(data.client);
                myClients.removeOne(data);
            }
            break;
        }
        case Network::MessageType_GameResults:{
			//Декодируем результаты и передаем их менеджеру
            System::String messageData = System::String(message.data, message.size);
            Gameplay::Game gameData = System::Serializer::deserialize<Gameplay::Game>(messageData);
            emit game_ended(gameData, this);
            break;
        }
        default:{
            emit on_message_got(message);
        }
        }

    }
}
