#include "gamemanager.h"
#include "include_s.h"
#include "RolesInclude.h"
#include "systemfunctions_s.h"
#include "singletonGM.h"
#include "networker_s.h"
#include "singletonNW.h"

using namespace Mafia;

NetWorker::NetWorker() {
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

GameManager::GameManager(){
    if(gameManagerSingleton == 0){
		netWorkerSingleton = new NetWorker();
        netWorkerSingleton->initServer();
        mesReceivingThread = std::thread(&NetWorker::processMessages, netWorkerSingleton);
        mesReceivingThread.detach();
		checkConThread = std::thread(&NetWorker::checkConnections, netWorkerSingleton);
		checkConThread.detach();
        //objectInitialized = true;
		gameManagerSingleton = this;
    } else{
        throw std::exception();
    }
}

void GameManager::initGame(){
    netWorkerSingleton->closeRoom();
    netWorkerSingleton->reorganizeClients();

    playersCount = netWorkerSingleton->getClientsCount();

    int* rolesCounts = new int[MAX_ROLE_ID];
    _setRolesCount(rolesCounts);
    int* roles = _shuffleRoles(rolesCounts);

    _setRoles(roles);
	currentState = currentState+1;
    netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)&currentState, 4);
}

void GameManager::_joinThreads() {
	if (checkConThread.joinable()) {
		checkConThread.join();
	}
	if (mesReceivingThread.joinable()) {
		mesReceivingThread.join();
	}
}


void GameManager::finish()
{

	std::thread fin(&GameManager::_joinThreads, this);
	fin.detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(TIME_PAUSE));
	netWorkerSingleton->finish();
}

GameManager::~GameManager()
{
    finish();
}

int* GameManager::_shuffleRoles(int* arr){
    int* rolesArr = new int[playersCount];
    int curIdx = 0;
    for(int i = 0; i < MAX_ROLE_ID; i++){
        for(int j = 0; j < arr[i]; j++){
            rolesArr[curIdx] = i;
            curIdx++;
        }
    }
    for(int i = 0; i < playersCount; i++){
        int f_ind = (unsigned int)rand() % playersCount;
        int s_ind = (unsigned int)rand() % playersCount;

        int tmp = rolesArr[f_ind];
        rolesArr[f_ind] = rolesArr[s_ind];
        rolesArr[s_ind] = tmp;
    }
    return rolesArr;
}

void GameManager::_setRoles(int *roles)
{
    for(int i = 0; i < playersCount; i++){
        switch(roles[i]){
        case CIVILLIAN_ROLE:{
            players[i] = new Civillian();
            netWorkerSingleton->sendMessage(i, ROLE_MESSAGE_ID, (char*)&roles[i], 4);
            break;
        }
        case MAFIA_ROLE:{
            players[i] = new MafiaR();
            break;
        }
        default:{
            break;
            players[i] = new NoneRole();
        }
        }
    }
}

int GameManager::_getFather(){
    for(int i = 0; i < playersCount; i++){
        if(players[i]->roleIdx() == MAFIA_ROLE && players[i]->alive()){
            return i;
        }
    }
    return -1;
}

int GameManager::_setRolesCount(int* arr){
    if(playersCount < 1){
        return -1;
    }
    if(playersCount < 4){
        arr[CIVILLIAN_ROLE] = playersCount;
        for(int i = 1; i < MAX_ROLE_ID; i++){
            arr[i] = 0;
        }
        return 0;
    }
    if(playersCount < 7){
        arr[CIVILLIAN_ROLE] = playersCount-1;
        arr[MAFIA_ROLE] = 1;
        for(int i = 2; i < MAX_ROLE_ID; i++){
            arr[i] = 0;
        }
        return 0;
    }
	return -1;
}

