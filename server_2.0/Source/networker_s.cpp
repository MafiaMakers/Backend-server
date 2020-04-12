#include "include_s.h"
#include "networker_s.h"
#include "systemfunctions_s.h"

#pragma warning(disable:4996)
namespace Mafia {

    //Constructor of NetWorker is in gamemanager.cpp

    //Initializes server at IP servIP, returns 0 if success, errorId if error (errorId from defines.h)
    int NetWorker::initServer(){
        roomId = (char)(rand() % 256);
        std::cout << "Room index - " << (int)roomId << std::endl;
        //Get errorId from binding
        int err = _tryBind();
        if(err){
            return err;
        }
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
    //this function doesn't work yet :((
    /*char* NetWorker::getMyIP(){
        char buf[BUF_SIZE];
        gethostname(buf, sizeof(buf));
        struct hostent *host = gethostbyname(buf);
        if(host == NULL)
        {
            return NULL;
        }
        unsigned char result[4];
        //Obtain the computer's IP
        result[0] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
        result[1] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
        result[2] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
        result[3] = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;
        std::cout << result << std::endl;
        return (char*)result;

    }
	*/

	//Run in another thread!
	void NetWorker::checkConnections() {
		while (true) {
			for (int i = 0; i < maxClientIndex; i++) {
				clients[i].connected = answered[i];
				if (!answered[i]) {
					std::cout << i << "th client disconnected" << std::endl;
				}
				answered[i] = false;
				std::cout << sendMessage(clients[i].clientAddr, CHECK_CONNECTION_MESAGE_ID, (char*)"check", 6) << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(TIME_PAUSE));
		}
	}

    int NetWorker::sendToAll(short messageId, char *message, int mesLen){
        int err = 0;
        for(int i = 0; i < maxClientIndex; i++){
            if(clients[i].connected){
                int err_tmp = sendMessage(clients[i].clientAddr, messageId, message, mesLen);
                if(err_tmp != 0){
                    err = err_tmp;
                }
            }
        }
        return err;
    }

    int NetWorker::getClientsCount(){
        return maxClientIndex;
    }

    //recommended to run in another thread
    int NetWorker::receiveMessage(){
        //buffer to write there received message
        char buffer[BUF_SIZE];
        zeroMemSys(buffer, BUF_SIZE);
        sockaddr_in currentClient;
        int cCSize = sizeof(currentClient);
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
                    sendMessage(currentClient, ERROR_MESSAGE_ID, (char*)&err, 4);
                    return(err);
                }
                //if error was in control sum - it means that not all message received, then ask for resending
                char resendMes[2];
                zeroMemSys(resendMes, 2);
                resendMes[0] = ((char*)&mId)[0];
                resendMes[1] = ((char*)&mId)[1];
                return(sendMessage(currentClient, RESEND_MESSAGE_ID, resendMes, 2));

            }
            int error = _processMessage(currentClient, mes, bytesReceived - 7, mId);
            if(error != 0){
                sendMessage(currentClient, ERROR_MESSAGE_ID, (char*)&error, 4);
                return error;
            }
            return(0);
        }
        else  {
            closesocket(sock);
            WSACleanup();
            return RECEIVING_ERROR;
        }
    }

    //a method to continuosly look for new messages [RUN IN ANOTHER THREAD!]
    int NetWorker::processMessages(){
        while(true){
            int err = receiveMessage();
            if(err != 0){
                std::cout << err << " - ERROR!" << std::endl;
                //return err;
            }
        }
    }

    //method to close room
    void NetWorker::closeRoom(){
        roomOpen = false;
    }

    //sends message with length mesLen and id messageId to client. Returns 0 if succes, error id if error
    int NetWorker::sendMessage(sockaddr_in client, short messageId, char* message, int mesLen){
        char resMes[BUF_SIZE];
        zeroMemSys(resMes, BUF_SIZE);
        int Idx = _clientIndex(client);
        if(Idx != -1){
            clients[Idx].lastMes.id = messageId;
            clients[Idx].lastMes.length = mesLen;
            clients[Idx].lastMes.message = message;

        }
        int err = _wrapMessage(message, mesLen, messageId, resMes);
        if(err != 0){
            return(err);
        }
        return(sendto(sock, resMes, mesLen + 7, 0, (sockaddr *)&client, sizeof(client)));
    }

    int NetWorker::sendMessage(int clientIdx, short messageId, char *message, int mesLen)
    {
        return sendMessage(clients[clientIdx].clientAddr, messageId, message, mesLen);
    }

    void NetWorker::reorganizeClients(){
        for(int i = 0; i < maxClientIndex; i++){
            if(clients[i].connected == false){
                for(int j = i+1; j < maxClientIndex; j++){
                    clients[j-1] = clients[j];
                }
                maxClientIndex--;
            }
        }
    }

    int NetWorker::_wrapMessage(char* message, int mesLen, short messageId, char* result){
        result[0] = roomId;
        result[1] = ((char*)&messageId)[0];
        result[2] = ((char*)&messageId)[1];

        int controlSum = 0;
        for(int i = 0; i < mesLen; i++){
            result[i+3] = message[i];
            controlSum += (int)((unsigned char)message[i]);
        }

        for(int i = 0; i < 4; i++){
            result[3 + mesLen + i] = ((char*)&controlSum)[i];
        }

        return(0);
    }

    int NetWorker::_clientIndex(sockaddr_in client){
        for(int i = 0; i < maxClientIndex; i++){
            if(clients[i].clientAddr.sin_addr.S_un.S_addr == client.sin_addr.S_un.S_addr && clients[i].clientAddr.sin_port == client.sin_port){
                return i;
            }
        }
        return -1;
    }

    //There will be process method, but it's the next step
    int NetWorker::_processMessage(sockaddr_in client, char* message, int size, short messageId){
        switch(messageId){
        case RESEND_MESSAGE_ID:{
            std::cout << "Message received: " << message << std::endl
                      << "Message ID: " << messageId << std::endl;
            if(size != 2){
                return MESSAGE_SIZE_ERROR;
            }
            int cInd = _clientIndex(client);
            if(cInd == -1){
                return PRIVACY_ERROR;
            }
            short mId = 0;
            mId = *((short*)(message));
            sendMessage(client, clients[cInd].lastMes.id,
                        clients[cInd].lastMes.message, clients[cInd].lastMes.length);
            break;
        }
        case (short)TEXT_MESSAGE_ID:{
            int idx = _clientIndex(client);
            if(idx == -1){
                return PRIVACY_ERROR;
            }
            std::cout << "Message received: " << message << std::endl
                      << "Message ID: " << messageId << std::endl;
            break;
        }
        case CONNECT_MESSAGE_ID:{
            if(!roomOpen){
                return CLOSED_ROOM_REQUEST_ERROR;
            }
            if(maxClientIndex < CLIENTS_MAX_COUNT){
                for(int i = 0; i < maxClientIndex; i++){
                    if(clients[i].clientAddr.sin_addr.S_un.S_addr == client.sin_addr.S_un.S_addr && client.sin_port == clients[i].clientAddr.sin_port){
                        return REPEATATIVE_REQUEST_ERROR;
                    }
                }
                clients[maxClientIndex] = Client();
                clients[maxClientIndex].name = new char[size];
                strcpy(clients[maxClientIndex].name, message);
                //clients[maxClientIndex].name = message;
                clients[maxClientIndex].lastMes = Message();
                clients[maxClientIndex].clientAddr = client;
                clients[maxClientIndex].connected = true;
                maxClientIndex++;
                sendMessage(client, SUCCESS_MESSAGE_ID, (char*)&roomId, 4);

                std::cout << clients[maxClientIndex-1].name << " joined room" << std::endl;
				answered[maxClientIndex - 1] = true;
            } else{
                return CLIENTS_LIMIT_ERROR;
            }
            break;
        }
        case SUCCESS_MESSAGE_ID:{
            std::cout << "Message received: " << message << std::endl
                    << "Message ID: " << messageId << std::endl;
            break;
        }
        case EXIT_ROOM_MESSAGE_ID:{
            int idx = _clientIndex(client);
            if(idx == -1){
                return PRIVACY_ERROR;
            }
            std::cout << clients[idx].name << " leaved room" << std::endl;
            clients[idx].connected = false;
            break;
        }
        case CHANGE_NAME_MESSAGE_ID:{
            int idx = _clientIndex(client);
            if(idx == -1){
                return PRIVACY_ERROR;
            }
            std::cout << clients[idx].name << " changed name to " << message << std::endl;
            //clients[idx].name = message;
            clients[idx].name = new char[size];
            strcpy(clients[idx].name, message);
            break;
        }
		case CHECK_CONNECTION_MESAGE_ID: {
			int idx = _clientIndex(client);
			if (idx != -1) {
				answered[idx] = true;
				return 0;
			}
			else {
				return PRIVACY_ERROR;
			}
			break;
		}
            default:{
                return UNKNOWN_MES_ID_ERROR;
                break;
            }
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

        //getting message Id
        short messageId = 0;
        ((char*)&messageId)[0] = bytes[1];
        ((char*)&messageId)[1] = bytes[2];

        if(messageId != CONNECT_MESSAGE_ID && messageId != RESEND_MESSAGE_ID){
            //check if roomId is ok
            char room = bytes[0];
            if(room != roomId){
                return ROOM_ID_ERROR;
            }
        }


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
