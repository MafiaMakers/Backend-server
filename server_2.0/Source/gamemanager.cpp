#include "gamemanager.h"
#include "singletonNW.h"
using namespace Mafia;


void IRole::setCanSpeakNow(bool val) {
	canSpeak = val;
	netWorkerSingleton->sendMessage(myIdx, CAN_SPEAK_MESSAGE_ID, (char*)& canSpeak, 1, myRoomId);
}

void GameManager::answer(int index) {
	for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == index) {
			players[i].answered = true;
			break;
		}
	}

}

GameManager::GameManager(){}

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

int GameManager::addPlayer(int index) {
	if (playersCount < PLAYERS_MAX_COUNT) {
		playersIndexes[playersCount] = index;
		playersCount++;
		//std::cout << playersCount << std::endl;
		return 0;
	}
	else {
		return CLIENTS_LIMIT_ERROR;
	}
}

void GameManager::_freePlayers() {
	for (int i = playersCount - 1; i >= 0; i--)
	{
		if (!netWorkerSingleton->isClientOnline(playersIndexes[i])) {
			netWorkerSingleton->throwClient(playersIndexes[i]);
			for (int j = i+1; j < playersCount; j++)
			{
				if (j == adminIdx) {
					adminIdx--;
				}
				players[j - 1] = players[j];
				playersIndexes[j - 1] = playersIndexes[j];
			}

			playersCount--;
			players[playersCount] = IRole();
		}
	}
	sendToAllInRoom(SET_ADMIN_MESSAGE_ID, (char*)& adminIdx, 4);
}

void GameManager::initGame(){
	roomOpened = true;
	_findNewAdmin();
	_freePlayers();
	gonext = false;
	while (!gonext) {
		std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
	}
	currentState = currentState+1;
	//std::cout << "success!!!" << std::endl;
	for (int i = 0; i < playersCount; i++)
	{
		netWorkerSingleton->sendMessage(playersIndexes[i], INDEX_MESSAGE_ID, (char*)& i, 4, myRoomId);
	}

    sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
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
			netWorkerSingleton->sendMessage(playersIndexes[(i + roundIndex) % playersCount], VOTE_MESSAGE_ID, (char*)"move for voting", 16, myRoomId);
			int votedFor = -1;
			players[(i + roundIndex) % playersCount].answered = false;
			for (int j = 0; j < TALK_TIME / DELTA_TIME; j++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
				if (players[(i + roundIndex) % playersCount].answered) {
					break;
				}
			}
			
			votedFor = players[(i + roundIndex) % playersCount].lastPlayerVotedIndex();
			if (votedFor != -1 && votedFor < playersCount) {
				if (players[votedFor].alive()) {
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
					sendToAllInRoom(MADE_VOTE_MESSAGE, result, 8);
				}
			}

			players[i].setCanSpeakNow(false);
		}
	}

	currentState++;
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)& currentState, 4);
	_sendCandidates();
}

int GameManager::getAdminIdx() {
	return adminIdx;
}

void GameManager::sendToAllAlive(short messageId, char* message, int mesLen) {
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			netWorkerSingleton->sendMessage((i + roundIndex) % playersCount, messageId, message, mesLen, myRoomId);
		}
	}
}

void GameManager::_findNewAdmin() {
	if (!netWorkerSingleton->isClientOnline(playersIndexes[adminIdx])) {
		for (int i = 0; i < playersCount; i++)
		{
			if (netWorkerSingleton->isClientOnline(playersIndexes[i])) {
				adminIdx = i;
				sendToAllInRoom(SET_ADMIN_MESSAGE_ID, (char*)& adminIdx, 4);
			}
		}
	}
}

int GameManager::getPlayerByIndex(int index) {
	return(playersIndexes[index]);
}

int GameManager::getPlayersCount() {
	return this->playersCount;
}

void GameManager::sendToAllInRoom(short messageId, char* message, int mesLen) {
	for (int i = 0; i < playersCount; i++)
	{
		netWorkerSingleton->sendMessage(playersIndexes[i], messageId, message, mesLen, myRoomId);
	}
}

void GameManager::setMyRoomId(int id) {
	myRoomId = id;
}

char* GameManager::initRoom() {
	roomCreated = true;
	roomOpened = true;
	for (int i = 0; i < KEY_SIZE; i++)
	{
		key[i] = 'A' + abs((char)random() % 26);
	}
	return key;
}

bool GameManager::isPlayerIn(int index) {
	for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == index) {
			return true;
		}
	}
	return false;
}

void GameManager::_sendCandidates() {
	int candidatesCount = 0;
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive() && players[i].isCandidate) {
			candidatesCount++;
		}
	}

	int* candidates = new int[candidatesCount];
	int curIdx = 0;
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive() && players[i].isCandidate) {
			candidates[curIdx] = i;
			curIdx++;
		}
	}
	sendToAllAlive(CANDIDATES_MESSAGE_ID, (char*)candidates, candidatesCount * 4);
}

void GameManager::deathStage(int deathRound){
	for (int i = 0; i < playersCount; i++) {
		players[i].setCanSpeakNow(false);
	}

	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			if (players[i].isCandidate) {
				players[i].answered = false;
				players[i].setCanSpeakNow(true);
				for (int j = 0; j < TALK_TIME / DELTA_TIME; j++)
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
		for (int j = 0; j < playersCount; j++)
		{
			if (players[j].alive()) {
				if (players[j].lastPlayerVotedIndex() == -1) {
					allAnswered = false;
					break;
				}
			}
		}
		if (allAnswered) {
			break;
		}
	}
	int defaultIndex = 0;
	for (int i = 0; i < playersCount; i++)
	{
		int idx = (i + roundIndex) % playersCount;
		if (players[idx].isCandidate) {
			defaultIndex = idx;
		}
	}

	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].alive()) {
			bool fine = false;
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
					sendToAllInRoom(MADE_VOTE_MESSAGE, result, 8);
					fine = true;
				}
			}
			if (!fine) {
				players[defaultIndex].votes++;
			}
		}
	}

	int max = 0;
	int maxIdx = -1;
	int secondMax = -1;
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].isCandidate) {
			if (players[i].votes == max) {
				secondMax = i;
			}
			if (players[i].votes > max) {
				secondMax = -1;
				max = players[i].votes;
				maxIdx = i;
			}
			
		}
	}
	if (secondMax != -1) {
		if (max == 0 || deathRound == 1) {
			maxIdx = -1;
		}
		else {
			for (int i = 0; i < playersCount; i++)
			{
				
				if (!players[i].isCandidate || players[i].votes != max) {
					players[i].isCandidate = false;
				}
				players[i].votes = 0;
			}
			sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
			_sendCandidates();
			deathStage(1);
			maxIdx = -1;
		}
	}

	if (maxIdx != -1) {
		bool changeFather = (maxIdx == _getFather());
		
		sendToAllInRoom(DIE_HEAL_MESSAGE_ID, players[maxIdx].die(), 5);

		if (changeFather) {
			int fatherId = _getFather();
			for (int i = 0; i < playersCount; i++)
			{
				if (players[i].roleIdx() == MAFIA_ROLE) {
					netWorkerSingleton->sendMessage(playersIndexes[i], FATHER_MESSAGE_ID, (char*)& fatherId, 4, myRoomId);
				}
			}
		}
	}
}

void GameManager::setAdmin(int index) {
	/*for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == index) {
			adminIdx = i;
			break;
		}
	}*/
	adminIdx = index;
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
						netWorkerSingleton->sendMessage(playersIndexes[j], VOTE_MESSAGE_ID, (char*)"kill", 5, myRoomId);
					}
					break;
				}
				case SHERIFF_ROLE: {
					foundRole = true;
					sheriffIdx = j;
					players[j].setLastPlayerVotedIndex(-1);
					netWorkerSingleton->sendMessage(playersIndexes[j], VOTE_MESSAGE_ID, (char*)"check", 6, myRoomId);
					break;
				}
				case DOCTOR_ROLE: {
					foundRole = true;
					doctorIdx = j;
					players[j].setLastPlayerVotedIndex(-1);
					netWorkerSingleton->sendMessage(playersIndexes[j], VOTE_MESSAGE_ID, (char*)"hill", 5, myRoomId);
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
			for (int j = 0; j < THINK_TIME / DELTA_TIME; j++)
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
			bool changeFather = (victim == _getFather());
			sendToAllInRoom(DIE_HEAL_MESSAGE_ID, players[victim].die(), 5);

			if (changeFather) {
				int fatherId = _getFather();
				for (int i = 0; i < playersCount; i++)
				{
					if (players[i].roleIdx() == MAFIA_ROLE) {
						netWorkerSingleton->sendMessage(playersIndexes[i], FATHER_MESSAGE_ID, (char*)& fatherId, 4, myRoomId);
					}
				}
			}
		}

	}

	if (sheriffIdx != -1) {
		int checkedIdx = players[sheriffIdx].lastPlayerVotedIndex();
		if (checkedIdx != -1 && checkedIdx < playersCount) {
			bool isDark = !players[checkedIdx].isRed();
			netWorkerSingleton->sendMessage(playersIndexes[sheriffIdx], SHERIFF_MESSAGE_ID, (char*) & (isDark), 1, myRoomId);
		}
	}

	currentState++;
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)& currentState, 4);
}

void GameManager::speakingStage(){
	gonext = false;
	//std::cout << "speaking stage started!" << std::endl;
	while (!gonext) {
		std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
	}
	currentState++;
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)& currentState, 4);
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
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)& currentState, 4);
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
		sendToAllInRoom(TEXT_MESSAGE_ID, (char*)"Mafia wins", 11);
	}
	else {
		sendToAllInRoom(TEXT_MESSAGE_ID, (char*)"Civilians win", 14);
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

bool GameManager::checkEmpty() {
	for (int i = 0; i < playersCount; i++)
	{
		if (netWorkerSingleton->isClientOnline(playersIndexes[i])) {
			return false;
		}
	}
	return true;
}

void GameManager::sendAudio(char* data, int size, int index) {
	int trueIndex = -1;
	for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == index) {
			trueIndex = i;
			break;
		}
	}

	if (trueIndex != -1 && (currentState == WAITING_STAGE || players[trueIndex].canSpeakNow())) {
		char* message = new char[size + 1];
		message[0] = (char)trueIndex;
		for (int i = 0; i < size; i++)
		{
			message[i + 1] = data[i];
		}

		for (int i = 0; i < playersCount; i++)
		{
			if ((currentState == WAITING_STAGE || players[i].canListenNow()) && i != trueIndex) {
				netWorkerSingleton->sendMessage(playersIndexes[i], AUDIO_MESSAGE_ID, message, size + 1, myRoomId);
			}
		}
	}
	
}

void GameManager::sendVideo(char* data, int size, int index) {
	int trueIndex = -1;
	for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == index) {
			trueIndex = i;
			break;
		}
	}

	if (trueIndex != -1) {
		char* message = new char[size + 1];
		message[0] = (char)trueIndex;
		for (int i = 0; i < size; i++)
		{
			message[i + 1] = data[i];
		}

		for (int i = 0; i < playersCount; i++)
		{
			if ((currentState == WAITING_STAGE || players[i].canListenNow()) && i != trueIndex) {
				netWorkerSingleton->sendMessage(playersIndexes[i], VIDEO_MESSAGE_ID, message, size + 1, myRoomId);
			}
		}
	}
}

void GameManager::finish()
{
	sendToAllInRoom(EXIT_ROOM_MESSAGE_ID, (char*)"closing", 8);
	for (int i = 0; i < playersCount; i++)
	{
		netWorkerSingleton->throwClient(playersIndexes[i]);
	}
	currentState = WAITING_STAGE;
	players = new IRole[CLIENTS_MAX_COUNT];
	playersCount = 0;
	roundIndex = 0;
	gonext = false;
	roomCreated = false;
	roomOpened = false;
	playersIndexes = new int[PLAYERS_MAX_COUNT];
	adminIdx = 0;
	myRoomId = 0;
	key = new char[KEY_SIZE];
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
		players[i].setMyIdx(playersIndexes[i]);
		players[i].setRoomId(myRoomId);
		netWorkerSingleton->sendMessage(playersIndexes[i], ROLE_MESSAGE_ID, (char*)& roles[i], 4, myRoomId);
    }

	int fatherId = _getFather();
	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].roleIdx() == MAFIA_ROLE) {
			netWorkerSingleton->sendMessage(playersIndexes[i], FATHER_MESSAGE_ID, (char*)& fatherId, 4, myRoomId);
		}
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
	for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == voterIdx) {
			players[i].setLastPlayerVotedIndex(playerIdx);
			break;
		}
	}
	
}

bool GameManager::isInitialized() {
	return roomCreated;
}

bool GameManager::isOpened() {
	return roomOpened;
}

char* GameManager::getKey() {
	return key;
}
