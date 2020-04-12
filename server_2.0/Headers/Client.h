
#ifndef CLIENT_H
#define CLIENT_H
#include "include_s.h"
namespace Mafia {

    struct Message{
    public:
        int id;
        int length;
        char* message;
    };

    struct Client{
    public:
        Client();
        sockaddr_in clientAddr;
        char * name;
        Message lastMes;
        bool connected;
    };
}

#endif // CLIENT_H
