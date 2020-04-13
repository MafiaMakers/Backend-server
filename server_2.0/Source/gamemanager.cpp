#include "gamemanager.h"
#include "include_s.h"
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

void NetWorker::_vote(int voterIdx, int playerIdx) {
	std::cout << voterIdx << " votes for " << playerIdx << std::endl;
	gameManagerSingleton->vote(voterIdx, playerIdx);
}

void IRole::die() {
	isAlive = false;
	netWorkerSingleton->sendMessage(myIdx, DIE_HILL_MESSAGE_ID, (char*)& isAlive, 1);
}

void IRole::hill() {
	isAlive = true;
	netWorkerSingleton->sendMessage(myIdx, DIE_HILL_MESSAGE_ID, (char*)& isAlive, 1);
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

void GameManager::argumentingStage() {
	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::deathStage(){
	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::nightStage(){
	int fatherIdx = _getFather();
	for (int i = 1; i < MAX_ROLE_ID; i++) {
		for (int j = 0; j < playersCount; j++) {
			if (players[j].roleIdx() == i) {
				players[j].setCanSpeakNow(true);
				players[j].setCanListenNow(true);
				switch (i) {
				case CIVILLIAN_ROLE: {
					break;
				}
				case MAFIA_ROLE: {
					if (j == fatherIdx) {
						players[j].setLastPlayerVotedIndex(-1);
						netWorkerSingleton->sendMessage(j, VOTE_MESSAGE_ID, (char*)"kill", 5);
					}
					break;
				}
				default:
					break;
				}
				
			}
			else {
				players[j].setCanSpeakNow(false);
				players[j].setCanListenNow(false);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(THINK_TIME));
		int victim = players[fatherIdx].lastPlayerVotedIndex();
		if (victim != -1) {
			players[victim].die();
		}
	}
}

void GameManager::speakingStage(){
	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::resultsStage(){
	finish();
}

bool GameManager::checkFinish() { return true; }

bool GameManager::gameCycle() {
	speakingStage();
	nightStage();
	argumentingStage();
	deathStage();
	return(checkFinish());
}

void GameManager::fullGame() {
	initGame();
	do {} while (!gameCycle());
	resultsStage();
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
	netWorkerSingleton->sendToAll(EXIT_ROOM_MESSAGE_ID, (char*)"closing", 8);
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
    for(int i = 0; i < playersCount*2; i++){
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
		players[i] = IRole(roles[i]);
		players[i].setMyIdx(i);
		netWorkerSingleton->sendMessage(i, ROLE_MESSAGE_ID, (char*)& roles[i], 4);
    }
}

int GameManager::_getFather(){
    for(int i = 0; i < playersCount; i++){
        if(players[i].roleIdx() == MAFIA_ROLE && players[i].alive()){
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
        arr[MAFIA_ROLE] = playersCount;
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

void GameManager::vote(int voterIdx, int playerIdx) {
	players[voterIdx].setLastPlayerVotedIndex(playerIdx);
}
