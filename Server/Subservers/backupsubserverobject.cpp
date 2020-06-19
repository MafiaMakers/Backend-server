#include "backupsubserverobject.h"
using namespace Mafia;
using namespace Subservers;


Mafia::Subservers::BackupSubserverObject::BackupSubserverObject(MainServerNetworker *networker, int port, const Mafia::System::String path,
    const Mafia::System::String processName, int checkInterval, int maxNotAnswering, const Mafia::System::String specialCommands)
    : SubserverObject(networker, port, path, processName, checkInterval, maxNotAnswering, specialCommands)
{

}

void BackupSubserverObject::check_connection()
{
    try {
        Message message = Message();
        message.type = MessageType_CheckConnection;
        message.client = myAddress;
        message.data = (SymbolType*)"c";
        message.size = 2;

        networker->send_message(message);
    } catch (Exception* exception) {
        switch(exception->get_id()){
        default:{
            exception->show();
        }
        }
    }
}

void BackupSubserverObject::message_from_server(Message message)
{
    if(message.client == this->myAddress){
        switch (message.type) {
        case MessageType_CheckConnection:{
            notAnsweringsCount = 0;
            break;
        }
        case MessageType_CheckConnectionServer:{
            check_connection();
            break;
        }
        default:{
            emit on_message_got(message);
        }
        }

    }
}
