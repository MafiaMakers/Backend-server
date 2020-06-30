#include "backupsubserverobject.h"
using namespace Mafia;
using namespace Subservers;

const System::String BackupSubserverObject::exePath = System::String("");
const System::String BackupSubserverObject::exeProcessName = System::String("");

Mafia::Subservers::BackupSubserverObject::BackupSubserverObject(Network::MainServerNetworker *networker, int port, int checkInterval, int maxNotAnswering, const Mafia::System::String specialCommands)
    : SubserverObject(networker, port, exePath, exeProcessName, checkInterval, maxNotAnswering, specialCommands)
{

}

void BackupSubserverObject::check_connection()
{
    try {
        Network::Message message = Network::Message();
        message.type = Network::MessageType_CheckConnection;
        message.client = myAddress;
        message.data = (Network::SymbolType*)"c";
        message.size = 2;

        networker->send_message(message);
    } catch (Exceptions::Exception* exception) {
        switch(exception->get_id()){
        default:{
            exception->show();
        }
        }
    }
}

void BackupSubserverObject::message_from_server(Network::Message message)
{
    if(message.client == this->myAddress){
        switch (message.type) {
        case Network::MessageType_CheckConnection:{
            notAnsweringsCount = 0;
            break;
        }
        case Network::MessageType_CheckConnectionServer:{
            check_connection();
            break;
        }
        default:{
            emit on_message_got(message);
        }
        }

    }
}
