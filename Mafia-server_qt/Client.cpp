#include <include_s.h>
#include <Client.h>

namespace Mafia{
    Client::Client(){
        connected = false;
        name = (char*)"";
        lastMes = Message();
        clientAddr = sockaddr_in();
    }
}
