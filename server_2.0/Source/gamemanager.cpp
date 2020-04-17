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

void NetWorker::_setupMessageProcessor(char* message) {
	int* result = new int[MAX_ROLE_ID];
	for (int i = 0; i < MAX_ROLE_ID; i++)
	{
		result[i] = (int)message[i];
	}
	gameManagerSingleton->setupRoles(result);
}

void NetWorker::_nextStageMessageProcessor(char* message, int size) {
	std::thread nextStageThread(&GameManager::nextStage, gameManagerSingleton);
	nextStageThread.detach();
}

void NetWorker::_vote(int voterIdx, int playerIdx) {
	std::cout << voterIdx << " votes for " << playerIdx << std::endl;
	gameManagerSingleton->vote(voterIdx, playerIdx);
}

void NetWorker::_answerPlayer(int index) {
	gameManagerSingleton->answer(index);
}

void IRole::setCanSpeakNow(bool val) {
	canSpeak = val;
	netWorkerSingleton->sendMessage(myIdx, CAN_SPEAK_MESSAGE_ID, (char*)& canSpeak, 1);
}


void IRole::die() {
	isAlive = false;
	char* res = new char[5];
	res[0] = (char)isAlive;
	for (int i = 0; i < 4; i++)
	{
		res[i + 1] = ((char*)& myIdx)[i];
	}
	netWorkerSingleton->sendToAll(DIE_HEAL_MESSAGE_ID, res, 5);
}

void IRole::hill() {
	isAlive = true;
	char* res = new char[5];
	res[0] = (char)isAlive;
	for (int i = 0; i < 4; i++)
	{
		res[i + 1] = ((char*)& myIdx)[i];
	}
	netWorkerSingleton->sendToAll(DIE_HEAL_MESSAGE_ID, res, 5);
}

void GameManager::answer(int index) {
	players[index].answered = true;
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

void GameManager::nextStage() {
	if (currentState == SPEAKING_STAGE) {
		gonext = true;
	}
	if (currentState == WAITING_STAGE) {
		initGame();
	}
}

void GameManager::setupRoles(int* roles) {
	std::cout << "setting roles up" << std::endl;
	_setRoles(_shuffleRoles(roles));
	gonext = true;
}

void GameManager::initGame(){
    netWorkerSingleton->closeRoom();
    netWorkerSingleton->reorganizeClients();
	gonext = false;
    playersCount = netWorkerSingleton->getClientsCount();

	while (!gonext) {
		std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
	}
	currentState = currentState+1;
	//std::cout << "success!!!" << std::endl;
	for (int i = 0; i < playersCount; i++)
	{
		netWorkerSingleton->sendMessage(i, INDEX_MESSAGE_ID, (char*)& i, 4);
	}

    netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)&currentState, 4);
	//std::cout << "finished!!!" << std::endl;
	std::thread fullGameThread(&GameManager::fullGame, this);
	fullGameThread.detach();
}

void GameManager::argumentingStage() {
	for (int i = 0; i < playersCount; i++) {
		players[i].setCanSpeakNow(false);
		players[i].isCandidate = false;
	}

	for (int i = 0; i < playersCount; i++)
	{
		if (players[(i + roundIndex) % playersCount].alive()) {
			players[(i + roundIndex) % playersCount].setCanSpeakNow(true);
			players[(i + roundIndex) % playersCount].setLastPlayerVotedIndex(-1);
			netWorkerSingleton->sendMessage((i + roundIndex) % playersCount, VOTE_MESSAGE_ID, (char*)"move for voting", 16);
			int votedFor = -1;
			players[(i + roundIndex) % playersCount].answered = false;
			for (int i = 0; i < TALK_TIME / DELTA_TIME; i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
				if (players[(i + roundIndex) % playersCount].answered) {
					break;
				}
			}
			
			votedFor = players[(i + roundIndex) % playersCount].lastPlayerVotedIndex();
			if (votedFor != -1 && votedFor < playersCount) {
				players[votedFor].isCandidate = true;
				char* result = new char[8];
				for (int j = 0; j < 4; j++)
				{
					result[j] = ((char*)& i)[j];
				}
				for (int j = 0; j < 4; j++)
				{
					result[j + 4] = ((char*)& votedFor)[j];
				}
				netWorkerSingleton->sendToAll(MADE_VOTE_MESSAGE, result, 8);
			}

			players[i].setCanSpeakNow(false);
		}
	}

	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::sendToAllAlive(short messageId, char* message, int mesLen) {
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			netWorkerSingleton->sendMessage(i, messageId, message, mesLen);
		}
	}
}

void GameManager::deathStage(){
	for (int i = 0; i < playersCount; i++) {
		players[i].setCanSpeakNow(false);
	}

	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			if (players[i].isCandidate) {
				players[i].answered = false;
				players[i].setCanSpeakNow(true);
				for (int i = 0; i < TALK_TIME / DELTA_TIME; i++)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
					if (players[i].answered) {
						break;
					}
				}

				players[i].setCanSpeakNow(false);
				players[i].votes = 0;
			}
			players[i].setLastPlayerVotedIndex(-1);
		}
		
	}

	sendToAllAlive(VOTE_MESSAGE_ID, (char*)"string up", 10);
	for (int i = 0; i < VOTE_TIME / DELTA_TIME; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
		bool allAnswered = true;
		for (int i = 0; i < playersCount; i++)
		{
			if (players[i].alive()) {
				if (players[i].lastPlayerVotedIndex() == -1) {
					allAnswered = false;
					break;
				}
			}
		}
		if (allAnswered) {
			break;
		}
	}

	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			int voteIdx = players[i].lastPlayerVotedIndex();
			if (voteIdx != -1 && voteIdx < playersCount) {
				if (players[voteIdx].isCandidate) {
					players[voteIdx].votes++;
					char* result = new char[8];
					for (int j = 0; j < 4; j++)
					{
						result[j] = ((char*)& i)[j];
					}
					for (int j = 0; j < 4; j++)
					{
						result[j + 4] = ((char*)& voteIdx)[j];
					}
					netWorkerSingleton->sendToAll(MADE_VOTE_MESSAGE, result, 8);

				}
			}
		}
	}

	int max = -1;
	int maxIdx = -1;
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].isCandidate) {
			if (players[i].votes > max) {
				max = players[i].votes;
				maxIdx = i;
			}
		}
	}
	if (maxIdx != -1) {
		players[maxIdx].die();
	}
	
	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::nightStage(){
	int fatherIdx = _getFather();
	int sheriffIdx = -1;
	int doctorIdx = -1;
	for (int i = 1; i < MAX_ROLE_ID; i++) {
		bool foundRole = false;
		for (int j = 0; j < playersCount; j++) {
			if (players[j].roleIdx() == i && players[j].alive()) {
				players[j].setCanSpeakNow(true);
				players[j].setCanListenNow(true);
				switch (i) {
				case CIVILIAN_ROLE: {
					break;
				}
				case MAFIA_ROLE: {
					foundRole = true;
					if (j == fatherIdx) {
						players[j].setLastPlayerVotedIndex(-1);
						netWorkerSingleton->sendMessage(j, VOTE_MESSAGE_ID, (char*)"kill", 5);
					}
					break;
				}
				case SHERIFF_ROLE: {
					foundRole = true;
					sheriffIdx = j;
					players[j].setLastPlayerVotedIndex(-1);
					netWorkerSingleton->sendMessage(j, VOTE_MESSAGE_ID, (char*)"check", 6);
					break;
				}
				case DOCTOR_ROLE: {
					foundRole = true;
					doctorIdx = j;
					players[j].setLastPlayerVotedIndex(-1);
					netWorkerSingleton->sendMessage(j, VOTE_MESSAGE_ID, (char*)"hill", 5);
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
		if (foundRole) {
			int idx = -1;
			switch (i) {
			case MAFIA_ROLE: {
				idx = fatherIdx;
				break;
			}
			case SHERIFF_ROLE: {
				idx = sheriffIdx;
				break;
			}
			case DOCTOR_ROLE: {
				idx = doctorIdx;
				break;
			}
			default: {
				break;
			}
			}
			for (int i = 0; i < THINK_TIME / DELTA_TIME; i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
				bool answered = false;
				
				//std::cout << idx << std::endl;
				if (idx != -1) {
					if (players[idx].lastPlayerVotedIndex() != -1) {
						//std::cout << "answered" << std::endl;
						answered = true;
					}
				}
				if (answered) {
					//std::cout << "breaking" << std::endl;
					break;
				}
			}
		}
		
	}

	int resHilled = -1;
	if (doctorIdx != -1) {
		int hilled = players[doctorIdx].lastPlayerVotedIndex();
		if (hilled != -1 && hilled < playersCount) {
			resHilled = hilled;
		}
	}

	int victim = players[fatherIdx].lastPlayerVotedIndex();
	if (victim != -1 && victim < playersCount) {
		if (victim != resHilled) {
			players[victim].die();
		}

	}

	if (sheriffIdx != -1) {
		int checkedIdx = players[sheriffIdx].lastPlayerVotedIndex();
		if (checkedIdx != -1 && checkedIdx < playersCount) {
			bool isDark = !players[checkedIdx].isRed();
			netWorkerSingleton->sendMessage(sheriffIdx, SHERIFF_MESSAGE_ID, (char*) & (isDark), 1);
		}
	}

	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::speakingStage(){
	gonext = false;
	//std::cout << "speaking stage started!" << std::endl;
	while (!gonext) {
		std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
	}
	currentState++;
	netWorkerSingleton->sendToAll(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::resultsStage(){
	finish();
}

int GameManager::gameCycle() {
	speakingStage();
	nightStage();
	argumentingStage();
	deathStage();
	currentState = SPEAKING_STAGE;
	return(_checkWin());
}

bool GameManager::canListen(int index) {
	return(players[index].canListenNow());
}

bool GameManager::canSpeak(int index) {
	return(players[index].canSpeakNow());
}

void GameManager::fullGame() {
	int res = 0;
	do {
		res = gameCycle();
	} while (res == 0);
	if (res == -1) {
		netWorkerSingleton->sendToAll(TEXT_MESSAGE_ID, (char*)"Mafia wins", 11);
	}
	else {
		netWorkerSingleton->sendToAll(TEXT_MESSAGE_ID, (char*)"Civilians win", 14);
	}
	resultsStage();
}

int GameManager::_checkWin() {
	int diff = 0;
	bool foundMafia = false;
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			diff += (int)(players[i].isRed()) * 2 - 1;
		}
		if (!players[i].isRed() && players[i].alive()) {
			foundMafia = true;
		}
	}
	if (!foundMafia) {
		return 1;
	}
	if (diff <= 0) {
		return -1;
	}
	return 0;
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
	//std::cout << "Shuffling" << std::endl;

    int* rolesArr = new int[playersCount];
    int curIdx = 0;
    for(int i = 0; i < MAX_ROLE_ID; i++){
        for(int j = 0; j < arr[i]; j++){
            rolesArr[curIdx] = i;
            curIdx++;
        }
    }
    for(int i = 0; i < playersCount*2; i++){
        int f_ind = random() % playersCount;
        int s_ind = random() % playersCount;
        int tmp = rolesArr[f_ind];
        rolesArr[f_ind] = rolesArr[s_ind];
        rolesArr[s_ind] = tmp;
    }
	//std::cout << "Shuffling finished" << std::endl;
    return rolesArr;
}

void GameManager::_setRoles(int *roles)
{
	//std::cout << "sending roles" << std::endl;
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
	if (playersCount == 4) {
		arr[CIVILIAN_ROLE] = 1;
		arr[MAFIA_ROLE] = 1;
		arr[SHERIFF_ROLE] = 1;
		arr[DOCTOR_ROLE] = 1;
		return 0;
	}
    if(playersCount < 7){
        arr[CIVILIAN_ROLE] = playersCount-1;
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
