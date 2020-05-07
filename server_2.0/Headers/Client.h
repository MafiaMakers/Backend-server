
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
        Message lastMes;
        bool connected;
		bool initialized;
    };
}

#endif // CLIENT_H
