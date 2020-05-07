#include "include_s.h"
#include "networker_s.h"
#include "systemfunctions_s.h"


#pragma warning(disable:4996)
namespace Mafia {
	NetWorker* netWorkerSingleton = 0;
	NetWorker::NetWorker() {
		for (int i = 0; i < CLIENTS_MAX_COUNT; i++)
		{
			clients[i] = Client();
			clients[i].initialized = false;
		}
		if (netWorkerSingleton == 0) {
			_initSocket();
			// Setup the TCP listening socket
			_setAddr();
			//objectInitialized = true;
			netWorkerSingleton = this;
		}
		else {
			throw std::exception();
		}

	}

	void NetWorker::_setupMessageProcessor(int rId, char* message) {
		int* result = new int[MAX_ROLE_ID];
		for (int i = 0; i < MAX_ROLE_ID; i++)
		{
			result[i] = *((int*)(message + i * 4));
		}
		if (rooms[rId].isInitialized()) {
			rooms[rId].setupRoles(result);
		}
	}
	
	void NetWorker::_nextStageMessageProcessor(int rId, char* message, int size) {
		std::cout << "nextStagethread" << std::endl;
		std::thread nextStageThread(&GameManager::nextStage, &rooms[rId], message, size);
		nextStageThread.detach();
	}

	void NetWorker::_vote(int rId, int voterIdx, int playerIdx) {
		std::cout << voterIdx << " votes for " << playerIdx << std::endl;
		rooms[rId].vote(voterIdx, playerIdx);
	}

	void NetWorker::_answerPlayer(int rId, int index) {
		rooms[rId].answer(index);
	}

    //Initializes server at IP servIP, returns 0 if success, errorId if error (errorId from defines.h)
    int NetWorker::initServer(){
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

	bool NetWorker::isClientOnline(int index) {
		if (index >= 0 && index < maxClientIndex) {
			return clients[index].initialized && clients[index].connected;
		}
		return false;
	}

	//Run in another thread!
	void NetWorker::checkConnections() {
		while (true) {
			for (int i = 0; i < maxClientIndex; i++) {
				if (clients[i].initialized) {
					clients[i].connected = answered[i];
					if (!answered[i]) {
						std::cout << i << "th client disconnected" << std::endl;
					}
					answered[i] = false;
					int clientRoom = getRoomIdByIndex(i);
					sendMessage(clients[i].clientAddr, CHECK_CONNECTION_MESAGE_ID, (char*)"check", 6, clientRoom);
				}
				
			}

			for (int i = 0; i < ROOMS_MAX_COUNT; i++)
			{
				if (rooms[i].isInitialized()) {
					if (rooms[i].checkEmpty()) {
						rooms[i].finish();
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(TIME_PAUSE));
		}
	}

    int NetWorker::sendToAll(short messageId, char *message, int mesLen){
        int err = 0;
        for(int i = 0; i < maxClientIndex; i++){
            if(clients[i].connected){
				int clientRoom = getRoomIdByIndex(i);
                int err_tmp = sendMessage(clients[i].clientAddr, messageId, message, mesLen, clientRoom);
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
        char* buffer = new char[BUF_SIZE];
        //zeroMemSys(buffer, BUF_SIZE);
        sockaddr_in currentClient;
        int cCSize = sizeof(currentClient);
        //receive message
        int bytesReceived = recvfrom(this->sock,buffer,BUF_SIZE,0,(sockaddr *)&currentClient,&cCSize);
        //check if message is OK
        if (bytesReceived > 0){
            char* mes = new char[BUF_SIZE];
            //zeroMemSys(mes, BUF_SIZE);
            short mId = 0;
			int room;
            int err = _decodeMessage(buffer, bytesReceived, mes, &mId, &room);
            if(err != 0){
                //if message is wrong, throw it away
                if(err != CONTROL_SUM_ERROR && err != SHORT_MESSAGE_ERROR){
                    //sendMessage(currentClient, ERROR_MESSAGE_ID, (char*)&err, 4, room);
					delete[] buffer;
					delete[] mes;
                    return(err);
                }
                //if error was in control sum - it means that not all message received, then ask for resending
				if (mId != VIDEO_MESSAGE_ID && mId != AUDIO_MESSAGE_ID && mId > 0 && mId < MAX_MESSAGE_ID) {
					char resendMes[2];
					zeroMemSys(resendMes, 2);
					resendMes[0] = ((char*)& mId)[0];
					resendMes[1] = ((char*)& mId)[1];
					delete[] buffer;
					delete[] mes;
					return(sendMessage(currentClient, RESEND_MESSAGE_ID, resendMes, 2, room));
				}
				delete[] buffer;
				delete[] mes;
				return err;
            }
            int error = _processMessage(currentClient, mes, bytesReceived - 7, mId, room);
            if(error != 0){
                sendMessage(currentClient, ERROR_MESSAGE_ID, (char*)&error, 4, room);
				delete[] buffer;
				delete[] mes;
                return error;
            }
			delete[] buffer;
			delete[] mes;
            return(0);
        }
        else  {
			delete[] buffer;
            return RECEIVING_ERROR;
        }
    }

    //a method to continuosly look for new messages [RUN IN ANOTHER THREAD!]
    int NetWorker::processMessages(){
        while(true){
			receiveMessage();
        }
    }

    //sends message with length mesLen and id messageId to client. Returns 0 if succes, error id if error
    int NetWorker::sendMessage(sockaddr_in client, short messageId, char* message, int mesLen, int roomId){
        char* resMes = new char[mesLen + 7];
        //zeroMemSys(resMes, BUF_SIZE);
        int Idx = _clientIndex(client);
        if(Idx != -1){
            clients[Idx].lastMes.id = messageId;
            clients[Idx].lastMes.length = mesLen;
            clients[Idx].lastMes.message = message;

        }
        int err = _wrapMessage(message, mesLen, messageId, resMes, roomId);
        if(err != 0){
			delete[] resMes;
            return(err);
        }
		//std::cout << roomId << " - rid " << messageId << " - mid "<< (int)message[0] << " " << resMes << " " << mesLen + 7 << std::endl;
		int bytesSent = sendto(sock, resMes, mesLen + 7, 0, (sockaddr*)& client, sizeof(client));
		delete[] resMes;
        return(bytesSent);
    }

	int NetWorker::sendToAdmin(short messageId, char* message, int mesLen, int roomId) {
		return sendMessage(rooms[roomId].getPlayerByIndex(rooms[roomId].getAdminIdx()), messageId, message, mesLen, roomId);
	}

    int NetWorker::sendMessage(int clientIdx, short messageId, char *message, int mesLen, int roomId)
    {
        return sendMessage(clients[clientIdx].clientAddr, messageId, message, mesLen, roomId);
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

    int NetWorker::_wrapMessage(char* message, int mesLen, short messageId, char* result, int roomId){
        result[0] = (char)roomId;
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

	int NetWorker::_activeClientsCount() {
		int res = 0;
		for (int i = 0; i < maxClientIndex; i++)
		{
			if (clients[i].connected) {
				res++;
			}
		}
		return res;
	}

	int NetWorker::getRoomIdByIndex(int index) {
		for (int i = 0; i < ROOMS_MAX_COUNT; i++)
		{
			if (rooms[i].isInitialized()) {
				if (rooms[i].isPlayerIn(index)) {
					return i;
				}
			}
		}
		return -1;
	}

	void NetWorker::throwClient(int index) {
		clients[index].initialized = false;
		clients[index].connected = false;
		clients[index].lastMes = Message();
		//clients[index].name = (char*)"";
		clients[index].clientAddr = sockaddr_in();
	}

	int NetWorker::_getFirstFreeIndex() {
		for (int i = 0; i < maxClientIndex; i++)
		{
			if (!clients[i].initialized) {
				return i;
			}
		}
		if (maxClientIndex >= CLIENTS_MAX_COUNT) {
			return -1;
		}
		maxClientIndex++;
		return maxClientIndex-1;

	}

	int NetWorker::_connectClient(sockaddr_in client, char roomId, char* key, char* name) {
		int freeId = _getFirstFreeIndex();
		if (freeId != -1) {

			for (int i = 0; i < maxClientIndex; i++) {
				if (clients[i].clientAddr.sin_addr.S_un.S_addr == client.sin_addr.S_un.S_addr && client.sin_port == clients[i].clientAddr.sin_port && clients[i].initialized) {
					return REPEATATIVE_REQUEST_ERROR;
				}
			}
			for (int i = 0; i < KEY_SIZE; i++)
			{
				if (key[i] != rooms[roomId].getKey()[i]) {
					sendMessage(client, ERROR_MESSAGE_ID, (char*)"invalid key!", 13, 0);
					return KEY_ERROR;
				}
			}
			clients[freeId] = Client();
			clients[freeId].initialized = true;
			//clients[freeId].name = name;
			//strcpy(clients[maxClientIndex].name, message);
			//clients[maxClientIndex].name = message;
			clients[freeId].lastMes = Message();
			clients[freeId].clientAddr = client;
			clients[freeId].connected = true;
			char id = (char)roomId;
			sendMessage(client, SUCCESS_MESSAGE_ID, &id, 1, roomId);
			int clientsCount = rooms[roomId].getPlayersCount();
			sendMessage(client, CLIENTS_COUNT_MESSAGE_ID, (char*)&clientsCount, 4, roomId);
			if (clientsCount == 0) {
				sendMessage(client, SET_ADMIN_MESSAGE_ID, (char*)& clientsCount, 4, roomId);
			}
			rooms[roomId].addPlayer(freeId, std::string(name));
			//maxClientIndex++;
			rooms[roomId].sendToAllInRoom(CLIENT_CONNECTED_DISCONNECTED_MESSAGE_ID, name, strlen(name));
			//sendToAll(CLIENT_CONNECTED_DISCONNECTED_MESSAGE_ID, message, size);
			std::cout << name << " joined room " << roomId << std::endl;
			answered[freeId] = true;
			return 0;
		}
		else {
			return CLIENTS_LIMIT_ERROR;
		}
	}

    //There will be process method, but it's the next step
    int NetWorker::_processMessage(sockaddr_in client, char* message, int size, short messageId, int roomId){
		int error = 0;
        switch(messageId){
        case RESEND_MESSAGE_ID:{
            std::cout << "Message received: " << message << std::endl
                      << "Message ID: " << messageId << std::endl;
            if(size != 2){
                error = MESSAGE_SIZE_ERROR;
				break;
            }
            int cInd = _clientIndex(client);
            if(cInd == -1){
				error = PRIVACY_ERROR;
				break;
            }
            short mId = 0;
            mId = *((short*)(message));
            sendMessage(client, clients[cInd].lastMes.id,
                        clients[cInd].lastMes.message, clients[cInd].lastMes.length, roomId);
            break;
        }
        case (short)TEXT_MESSAGE_ID:{
            int idx = _clientIndex(client);
            if(idx == -1){
				error = PRIVACY_ERROR;
				break;
            }
            std::cout << "Message received: " << message << std::endl
                      << "Message ID: " << messageId << std::endl;
            break;
        }
        case CONNECT_MESSAGE_ID:{
			char rId = message[0];
			char* key = message + 1;
			if (rId < 0 || rId >= ROOMS_MAX_COUNT) {
				error = ROOM_ID_ERROR;
				break;
			}
            if(!rooms[rId].isOpened()){
				error = CLOSED_ROOM_REQUEST_ERROR;
				break;
            }
			if (size <= KEY_SIZE + 1) {
				error = SHORT_MESSAGE_ERROR;
				break;
			}
			char* name = message + KEY_SIZE + 1;
			return _connectClient(client, rId, key, name);
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
				error = PRIVACY_ERROR;
				break;
            }
            //std::cout << clients[idx].name << " leaved room" << std::endl;
            clients[idx].connected = false;
            break;
        }
        case CHANGE_NAME_MESSAGE_ID:{
            int idx = _clientIndex(client);
            if(idx == -1){
				error = PRIVACY_ERROR;
				break;
            }
            //std::cout << clients[idx].name << " changed name to " << message << std::endl;
            //clients[idx].name = message;
			rooms[roomId].changeName(idx, std::string(message));
            //clients[idx].name = new char[size];
           // clients[idx].name = message;
            break;
        }
		case CHECK_CONNECTION_MESAGE_ID: {
			int idx = _clientIndex(client);
			if (idx != -1) {
				answered[idx] = true;
				error = 0;
				break;
			}
			else {
				error = PRIVACY_ERROR;
				break;
			}
			break;
		}
		case VOTE_MESSAGE_ID: {
			int idx = _clientIndex(client);
			if (idx == -1) {
				error = PRIVACY_ERROR;
				break;
			}
			if (size < 4) {
				error = SHORT_MESSAGE_ERROR;
				break;
			}
			int index = 0;
			for (int i = 0; i < 4; i++)
			{
				((char*)& index)[i] = message[i];
			}
			_vote(roomId, idx, index);
			break;
		}
		case NEXT_STAGE_MESSAGE_ID: {
			std::cout << "Next stage message id!" << std::endl;
			int idx = _clientIndex(client);
			if (idx != rooms[roomId].getPlayerByIndex(rooms[roomId].getAdminIdx())) {
				error = PRIVACY_ERROR;
				break;
			}
			_nextStageMessageProcessor(roomId, message, size);
			break;
		}
		case SET_ADMIN_MESSAGE_ID: {
			int idx = _clientIndex(client);
			if (idx != rooms[roomId].getPlayerByIndex(rooms[roomId].getAdminIdx())) {
				error = PRIVACY_ERROR;
				break;
			}
			if (size < 4) {
				error = SHORT_MESSAGE_ERROR;
				break;
			}
			int newIdx = *(int*)message;
			if (newIdx < maxClientIndex && newIdx >= 0) {
				if (clients[newIdx].connected) {
					rooms[roomId].setAdmin(newIdx);
					rooms[roomId].sendToAllInRoom(SET_ADMIN_MESSAGE_ID, message, size);
				}
				else {
					error = CLIENT_INDEX_ERROR;
					break;
				}
			}
			else {
				error = CLIENT_INDEX_ERROR;
				break;
			}
			break;
		}
		case SETUP_MESSAGE_ID: {
			std::cout << "Setup mesage id" << std::endl;
			int idx = _clientIndex(client);
			if (idx != rooms[roomId].getPlayerByIndex(rooms[roomId].getAdminIdx())) {
				error = PRIVACY_ERROR;
				break;
			}
			if (size != MAX_ROLE_ID * 4) {
				std::cout << "there error " << size << std::endl;
				error = SHORT_MESSAGE_ERROR;
				break;
			}
			_setupMessageProcessor(roomId, message);
			break;
		}
		case STOP_SPEAK_MESSAGE_ID: {
			int index = _clientIndex(client);
			if (index < maxClientIndex && index > -1) {
				_answerPlayer(roomId, index);
			}
			break;
		}
		case CREATE_ROOM_MESSAGE_ID: {
			int idx = _clientIndex(client);
			if (idx != -1) {
				error = REPEATATIVE_REQUEST_ERROR;
				break;
			}
			int i;
			for (i = 0; i < ROOMS_MAX_COUNT; i++)
			{
				if (!rooms[i].isInitialized()) {
					break;
				}
			}
			char* key = rooms[i].initRoom();
			rooms[i].setMyRoomId(i);
			int err = _connectClient(client, i, key, message);
			if (err != 0) {
				std::cout << "error " << err << std::endl;
			}
			else {
				rooms[i].setMyRoomId(i);
				sendMessage(client, KEY_MESSAGE_ID, key, KEY_SIZE, i);
				std::cout << "room created " << i << std::endl;
			}
			int res = 1;
			//sendMessage(client, RESULTS_MESSAGE_ID, (char*)&res, 4, roomId);
			break;
		}
		case VIDEO_MESSAGE_ID: {
			int idx = _clientIndex(client);
			if (idx == -1) {
				error = PRIVACY_ERROR;
				break;
			}
			rooms[roomId].sendVideo(message, size, idx);
			break;
		}
		case AUDIO_MESSAGE_ID: {
			int idx = _clientIndex(client);
			if (idx == -1) {
				error = PRIVACY_ERROR;
				break;
			}
			rooms[roomId].sendAudio(message, size, idx);
			break;
		}
		case DONT_PLAY_MESSAGE_ID: {
			rooms[roomId].setNotPlayers((int*)message, size / 4);
			break;
		}
        default:{
			error = UNKNOWN_MES_ID_ERROR;
            break;
        }
        }
		//delete[] message;
        return(error);
    }


    /*
    **  decodes message recieved from client using our message proto. Returns 0 if success, return error id (from defines.h) if error
    **  bytes is received message, size is size of message, result is INITIALIZED array with size BUF_SIZE of char to put there decoded message,
    **  getMessageId is ref to short variable to write there the id of this message
    */
    int NetWorker::_decodeMessage(char* bytes, int size, char* result, short* getMessageId, int* resultRoomId){
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
            if(room >= ROOMS_MAX_COUNT){
                return ROOM_ID_ERROR;
            }
			*resultRoomId = room;
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
        myAddr.sin_addr.S_un.S_addr = INADDR_ANY;
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
