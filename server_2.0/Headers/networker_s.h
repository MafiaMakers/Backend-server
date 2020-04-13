

#ifndef NETWORKER_H
#define NETWORKER_H
#include "include_s.h"
#include "Client.h"

namespace Mafia {

//the main networker class, singleton
    class NetWorker{
    public:
        NetWorker();
        //Initializes server at IP servIP, returns 0 if success, errorId if error (errorId from defines.h)
        int initServer();

        SOCKET getSock();

        char* getIP();

        sockaddr* getAddr();

        sockaddr_in* getAddrIn();

		void checkConnections();

        /*char* getMyIP();*/

        int sendToAll(short messageId, char* message, int mesLen);
        //recommended to run in another thread
        int receiveMessage();
        //sends message with length mesLen and id messageId to client. Returns 0 if succes, error id if error
        int sendMessage(sockaddr_in client, short messageId, char* message, int mesLen);

        int sendMessage(int clientIdx, short messageId, char* message, int mesLen);

        void finish();
        void closeRoom();
        int processMessages();

        void reorganizeClients();

        int getClientsCount();

        ~NetWorker();
    protected:
        //static bool objectInitialized;
        int _wrapMessage(char* message, int mesLen, short messageId, char* result);
        //There will be process method, but it's the next step
        int _processMessage(sockaddr_in client, char* message, int size, short messageId);
        int _clientIndex(sockaddr_in client);
        /*decodes message recieved from client using our message proto. Returns 0 if success, return error id (from defines.h) if error
        **bytes is received message, size is size of message, result is INITIALIZED array with size BUF_SIZE of char to put there decoded message,
        ** getMessageId is ref to short variable to write there the id of this message*/
        int _decodeMessage(char* bytes, int size, char* result, short* getMessageId);
        void _initSocket();

        void _setAddr();
        int _tryBind();

		void _vote(int voterIdx, int playerIdx);

        char roomId = 0;
        //Current server socket
        SOCKET sock;
        //This server IP
        std::string ipServer;
        //Current address
        sockaddr_in myAddr;
        //Clients
        Client clients[CLIENTS_MAX_COUNT];
        int maxClientIndex = 0;

		bool answered[CLIENTS_MAX_COUNT];

        bool roomOpen = true;


    };


}
#endif // NETWORKER_H
