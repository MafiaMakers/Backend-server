#include "include_s.h"

#ifndef CLIENT_H
#define CLIENT_H

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
