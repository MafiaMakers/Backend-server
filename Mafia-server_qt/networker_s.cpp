#include "include_s.h"
#include "networker_s.h"
#include "systemfunctions_s.h"

namespace Mafia {
    NetWorker::NetWorker(){
        _initSocket();
        // Setup the TCP listening socket
        _setAddr();
    }
    //Initializes server at IP servIP, returns 0 if success, errorId if error (errorId from defines.h)
    int NetWorker::initServer(){
        //Get errorId from binding
        int err = _tryBind();
        if(err){
            return err;
        }
        //std::cout << (int)(getMyIP()[0]) << std::endl;
        return 0;

    }

    SOCKET NetWorker::getSock(){
        return(sock);
    }

    char* NetWorker::getIP(){
        return((char*)ipServer.c_str());
    }

    sockaddr* NetWorker::getAddr(){
        return((sockaddr*)&myAddr);
    }

    sockaddr_in* NetWorker::getAddrIn(){
        return(&myAddr);
    }

    char* NetWorker::getMyIP(){
        char buf[BUF_SIZE];
        std::cout << "q";
        gethostname(buf, sizeof(buf));
        std::cout << "w";
        struct hostent *host = gethostbyname(buf);
        std::cout << "e";
        if(host == NULL)
        {
            return NULL;
        }
        unsigned char result[4];
        //zeroMemSys(result, 4);
        //Obtain the computer's IP
        result[0] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
        result[1] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
        result[2] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
        result[3] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;
        std::cout << result << std::endl;
        return (char*)result;

    }

    //recommended to run in another thread
    int NetWorker::receiveMessage(){
        //buffer to write there received message
        char buffer[BUF_SIZE];
        zeroMemSys(buffer, BUF_SIZE);
        sockaddr_in currentClient;
        //std::cout << this->sock << " " << this->roomId << " " << this->roomOpen << std::endl;
        int cCSize = sizeof(currentClient);
        //std::lock_guard<std::mutex>guard(lock);
        //receive message
        int bytesReceived = recvfrom(this->sock,buffer,BUF_SIZE,0,(sockaddr *)&currentClient,&cCSize);
        //check if message is OK
        if (bytesReceived > 0) {
            char mes[BUF_SIZE];
            zeroMemSys(mes, BUF_SIZE);
            short mId = 0;
            int err = _decodeMessage(buffer, bytesReceived, mes, &mId);
            if(err != 0){
                //if message is wrong, throw it away
                if(err != CONTROL_SUM_ERROR){
                    return(err);
                }
                //if error was in control sum - it means that not all message received, then ask for resending
                char resendMes[2];
                zeroMemSys(resendMes, 2);
                resendMes[0] = ((char*)&mId)[0];
                resendMes[1] = ((char*)&mId)[1];
                return(sendMessage(currentClient, RESEND_MESSAGE_ID, resendMes, 2));

            }
            return(_processMessage(currentClient, mes, bytesReceived-7, mId));
        }
        else  {
            closesocket(sock);
            WSACleanup();
            return RECEIVING_ERROR;
        }
    }

    void NetWorker::closeRoom(){
        //std::lock_guard<std::mutex>guard(lock);
        roomOpen = false;
    }

    //sends message with length mesLen and id messageId to client. Returns 0 if succes, error id if error
    int NetWorker::sendMessage(sockaddr_in client, short messageId, char* message, int mesLen){
        char resMes[BUF_SIZE];
        zeroMemSys(resMes, BUF_SIZE);
        int err = _wrapMessage(message, mesLen, messageId, resMes);
        if(err != 0){
            return(err);
        }
        return(sendto(sock, resMes, mesLen + 7, 0, (sockaddr *)&client, sizeof(client)));
    }

    int NetWorker::_wrapMessage(char* message, int mesLen, short messageId, char* result){
        result[0] = roomId;
        result[1] = ((char*)&messageId)[0];
        result[2] = ((char*)&messageId)[1];

        int controlSum = 0;
        for(int i = 0; i < mesLen; i++){
            result[i+3] = message[i];
            controlSum += (int)message[i];
        }

        for(int i = 0; i < 4; i++){
            result[3 + mesLen + i] = ((char*)&controlSum)[i];
        }

        return(0);
    }

    //There will be process method, but it's the next step
    int NetWorker::_processMessage(sockaddr_in client, char* message, int size, short messageId){
        switch(messageId){
        case 1:
            std::cout << "Message received: " << message << std::endl << "Message ID: " << messageId << std::endl;
            break;
        case 2:
            std::cout << "Message received: " << message << std::endl << "Message ID: " << messageId << std::endl;
            break;
        case 3:
            if(!roomOpen){
                int err = CLOSED_ROOM_REQUEST_ERROR;
                sendMessage(client, ERROR_MESSAGE_ID, (char*)&err, 4);
                return err;
            }
            if(maxClientIndex < CLIENTS_MAX_COUNT){
                for(int i = 0; i < maxClientIndex; i++){
                    if(clients[i].clientAddr.sin_addr.S_un.S_addr == client.sin_addr.S_un.S_addr){
                        return REPEATATIVE_REQUEST_ERROR;
                    }
                }
                clients[maxClientIndex].name = message;
                clients[maxClientIndex].lastMes = Message();
                clients[maxClientIndex].clientAddr = client;
                maxClientIndex++;
                std::cout << clients[maxClientIndex-1].name << std::endl;
            } else{
                return CLIENTS_LIMIT_ERROR;
            }
            break;
        default:
            return UNKNOWN_MES_ID_ERROR;
            break;
        }
        return(0);
    }


    /*
    **  decodes message recieved from client using our message proto. Returns 0 if success, return error id (from defines.h) if error
    **  bytes is received message, size is size of message, result is INITIALIZED array with size BUF_SIZE of char to put there decoded message,
    **  getMessageId is ref to short variable to write there the id of this message
    */
    int NetWorker::_decodeMessage(char* bytes, int size, char* result, short* getMessageId){
        //check if message is too short
        if(size < 8){
            return SHORT_MESSAGE_ERROR;
        }

        //check if roomId is ok
        char room = bytes[0];
        if(room != roomId){
            return ROOM_ID_ERROR;
        }

        //getting message Id
        short messageId = 0;
        ((char*)&messageId)[0] = bytes[1];
        ((char*)&messageId)[1] = bytes[2];

        //check if control sum is correct
        unsigned int controlSum = 0;
        for(int i = 0; i < 4; i++){
            ((char*)&controlSum)[i] = bytes[size - 4 + i];
        }

        unsigned int realSum = 0;

        for(int i = 3; i < size - 4; i++){
            realSum += (unsigned char)bytes[i];
        }

        if(realSum != controlSum){
            return CONTROL_SUM_ERROR;
        }

        //fill the result array
        for(int i = 3; i < size - 4; i++){
            result[i-3] = bytes[i];
        }
        *getMessageId = messageId;
        return 0;
    }


    void NetWorker::_initSocket(){
        WSADATA wsaData;
        // Initialize Winsock
        WSAStartup(MAKEWORD(2,2), &wsaData);
        // Create a SOCKET for connecting to server
        sock = socket(IP_PROTO, SOCK_DGRAM, IPPROTO_UDP);
    }

    void NetWorker::_setAddr(){
        myAddr.sin_family = IP_PROTO;
        myAddr.sin_addr.S_un.S_addr = 0;
        //myAddr.sin6_addr.u.Byte = //INADDR_ANY;//inet_addr(ipServer.c_str());
        //std::cout << inet_ntoa(myAddr.sin_addr) << std::endl;
        myAddr.sin_port=htons(CASUAL_PORT);
    }

    void NetWorker::finish(){
        closesocket(sock);
        WSACleanup();
    }

    int NetWorker::_tryBind(){
        int err;
        err = bind(sock, (sockaddr *) &myAddr, sizeof(*(sockaddr*)&myAddr));
        std::cout << "Err - " << err << std::endl;
        if (err == SOCKET_ERROR) {
            std::cout << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return SERVER_INITIALIZATION_ERROR;
        }
        return(0);
    }

    NetWorker::~NetWorker(){
        finish();
    }

}
