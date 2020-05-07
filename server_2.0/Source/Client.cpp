#include "include_s.h"
#include "Client.h"

namespace Mafia{
    Client::Client(){
        connected = false;
        lastMes = Message();
        clientAddr = sockaddr_in();
    }
}
