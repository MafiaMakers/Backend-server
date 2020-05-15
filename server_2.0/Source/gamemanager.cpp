#include "gamemanager.h"
#include "singletonNW.h"
#include <vector>
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

void GameManager::nextStage(char* message, int size) {
	if (currentState == SPEAKING_STAGE) {
		gonext = true;
	}
	if (currentState == WAITING_STAGE) {
		initGame();
	}
}

void GameManager::changeName(int index, std::string name) {
	int trueIndex = -1;
	for (int i = 0; i < playersCount; i++)
	{
		if (playersIndexes[i] == index) {
			trueIndex = i;
			break;
		}
	}
	if (trueIndex > -1 && trueIndex < playersCount) {
		players[trueIndex].setName(name);
		char* message = new char[name.length() + 1];
		std::cout << name << std::endl;
		message[0] = (char)trueIndex;
		for (int i = 0; i < name.length(); i++)
		{
			message[i + 1] = name[i];
		}
		sendToAllInRoom(CHANGE_NAME_MESSAGE_ID, message, name.length() + 1);
	}
}

void GameManager::setupRoles(int* roles) {
	std::cout << "setting roles up" << std::endl;
	for (int i = 0; i < MAX_ROLE_ID; i++)
	{
		//std::cout << roles[i] << std::endl;;
	}
	_setRoles(_shuffleRoles(roles));
	gonext = true;
}

int GameManager::addPlayer(int index, std::string name) {
	std::string message;
	message += (char)playersCount;
	for (int i = 0; i < playersCount; i++)
	{
		message += players[i].getName() + "\n";
	}
	if (playersCount < PLAYERS_MAX_COUNT) {
		
		netWorkerSingleton->sendMessage(index, CLIENTS_INFO_MESSAGE_ID, (char*)message.c_str(), message.length(), myRoomId);
		playersIndexes[playersCount] = index;
		players[playersCount].setName(name);
		std::cout << players[playersCount].getName() << std::endl;
		netWorkerSingleton->sendMessage(index, CLIENT_INDEX_MESSAGE_ID, (char*)&playersCount, 4, myRoomId);
		playersCount++;
		//std::cout << playersCount << std::endl;
		return 0;
	}
	else {
		return CLIENTS_LIMIT_ERROR;
	}
}

void GameManager::freePlayers() {
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
	/*for (int i = 0; i < playersCount; i++)
	{
		int fIdx = abs(random() % playersCount);
		int sIdx = abs(random() % playersCount);

		IRole cp = players[fIdx];
		players[fIdx] = players[sIdx];
		players[sIdx] = cp;

		if (fIdx == adminIdx || sIdx == adminIdx) {
			adminIdx = fIdx + sIdx - adminIdx;
		}

		int ci = playersIndexes[fIdx];
		playersIndexes[fIdx] = playersIndexes[sIdx];
		playersIndexes[sIdx] = ci;
	}*/
	std::string message = "";
	message += (char)playersCount;
	for (int i = 0; i < playersCount; i++)
	{
		message += players[i].getName() + "\n";
	}
	sendToAllInRoom(CLIENTS_INFO_MESSAGE_ID, (char*)message.c_str(), message.length());

	for (int i = 0; i < playersCount; i++)
	{
		netWorkerSingleton->sendMessage(playersIndexes[i], CLIENT_INDEX_MESSAGE_ID, (char*)&i, 4, myRoomId);
	}
	sendToAllInRoom(SET_ADMIN_MESSAGE_ID, (char*)&adminIdx, 4);
	
}

void GameManager::setNotPlayers(int* notPlayers, int size) {
	for (int i = 0; i < size; i++)
	{
		std::string name = players[notPlayers[i]].getName();
		players[notPlayers[i]] = IRole(NONE_ROLE);
		players[notPlayers[i]].setName(name);
		std::cout << players[notPlayers[i]].getName() << std::endl;
		players[notPlayers[i]].setMyIdx(playersIndexes[notPlayers[i]]);
		players[notPlayers[i]].setRoomId(myRoomId);
		players[notPlayers[i]].die();
		char* mes = new char[5];
		mes[0] = (char)true;
		for (int j = 0; j < 4; j++)
		{
			mes[j + 1] = ((char*)&notPlayers[i])[j];
		}
		sendToAllInRoom(DIE_HEAL_MESSAGE_ID, mes, 5);
		delete[] mes;
	}
}

void GameManager::initGame(){
	roomOpened = false;
	
	_findNewAdmin();
	freePlayers();
	netWorkerSingleton->sendMessage(playersIndexes[adminIdx], START_GAME_MESSAGE_ID, (char*)"A", 2, myRoomId);
	std::string message = "";
	message += (char)playersCount;
	for (int i = 0; i < playersCount; i++)
	{
		message += players[i].getName() + "\n";
	}
	sendToAllInRoom(CLIENTS_INFO_MESSAGE_ID, (char*)message.c_str(), message.length());
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

	std::thread fullGameThread(&GameManager::fullGame, this);
	fullGameThread.detach();
	std::cout << "finished" << std::endl;
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
				if (!roomCreated) {
					return;
				}
				if (players[(i + roundIndex) % playersCount].answered) {
					break;
				}
			}
			if (roomCreated) {
				votedFor = players[(i + roundIndex) % playersCount].lastPlayerVotedIndex();
				if (votedFor != -1 && votedFor < playersCount) {
					if (players[votedFor].alive()) {
						players[votedFor].isCandidate = true;
						char* result = new char[8];
						for (int j = 0; j < 4; j++)
						{
							result[j] = ((char*)&i)[j];
						}
						for (int j = 0; j < 4; j++)
						{
							result[j + 4] = ((char*)&votedFor)[j];
						}
						sendToAllInRoom(MADE_VOTE_MESSAGE, result, 8);
					}
				}
			}
			

			players[i].setCanSpeakNow(false);
		}
		else {
			//std::cout << "player " << i << " is dead" << std::endl;
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
					if (!roomCreated) {
						return;
					}
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
	currentState++;
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
	for (int i = 0; i < playersCount; i++)
	{
		players[i].setCanListenNow(true);
		players[i].setCanSpeakNow(true);
	}

	for (int i = 0; i < playersCount; i++)
	{
		if (players[i].isCandidate) {
			sendToAllAlive(VOTE_MESSAGE_ID, (char*)&i, 4);
			for (int i = 0; i < VOTE_TIME / DELTA_TIME; i++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
				if (!roomCreated) {
					return;
				}
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
		}
		for (int j = 0; j < playersCount; j++)
		{
			if (players[j].alive() && players[j].lastPlayerVotedIndex() == i) {
				char* result = new char[8];
				for (int k = 0; k < 4; k++)
				{
					result[k] = ((char*)&i)[k];
				}
				for (int k = 0; k < 4; k++)
				{
					result[k + 4] = ((char*)&i)[k];
				}
				//sendToAllInRoom(MADE_VOTE_MESSAGE, result, 8);
			}
		}
	}

	sendToAllInRoom(FINISH_VOTING_MESSAGE_ID, (char*)"a", 2);
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
			currentState = DEATH_STAGE;
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
				if (!roomCreated) {
					return;
				}
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
	if (roomCreated) {
		int resHilled = -1;
		if (doctorIdx != -1) {
			int hilled = players[doctorIdx].lastPlayerVotedIndex();
			if (hilled != -1 && hilled < playersCount) {
				resHilled = hilled;
			}
		}

		if (fatherIdx != -1) {
			int victim = players[fatherIdx].lastPlayerVotedIndex();
			if (victim != -1 && victim < playersCount) {
				if (victim != resHilled) {
					bool changeFather = (victim == _getFather());
					//std::cout << "kill " << victim << std::endl;
					sendToAllInRoom(DIE_HEAL_MESSAGE_ID, players[victim].die(), 5);

					if (changeFather) {
						int fatherId = _getFather();
						for (int i = 0; i < playersCount; i++)
						{
							if (players[i].roleIdx() == MAFIA_ROLE) {
								netWorkerSingleton->sendMessage(playersIndexes[i], FATHER_MESSAGE_ID, (char*)&fatherId, 4, myRoomId);
							}
						}
					}
				}

			}
		}
		

		if (sheriffIdx != -1) {
			int checkedIdx = players[sheriffIdx].lastPlayerVotedIndex();
			if (checkedIdx != -1 && checkedIdx < playersCount) {
				bool isDark = !players[checkedIdx].isRed();
				netWorkerSingleton->sendMessage(playersIndexes[sheriffIdx], SHERIFF_MESSAGE_ID, (char*)&(isDark), 1, myRoomId);
			}
		}

		currentState++;
		sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
	}
}

void GameManager::speakingStage(){
	gonext = false;
	for (int i = 0; i < playersCount; i++)
	{
		players[i].setCanSpeakNow(true);
		players[i].setCanListenNow(true);
		bool can = true;
		netWorkerSingleton->sendMessage(playersIndexes[i], CAN_SPEAK_MESSAGE_ID, (char*)&(can), 1, myRoomId);
	}
	//std::cout << "speaking stage started!" << std::endl;
	while (!gonext) {
		if (!roomCreated) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(DELTA_TIME));
	}
	if (roomCreated) {
		std::cout << "speaking stage finished" << std::endl;
		currentState++;
		sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
	}
}

void GameManager::resultsStage(){
	freePlayers();
	currentState = WAITING_STAGE;
	roomOpened = true;
	roundIndex = 0;
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
	for (int i = 0; i < playersCount; i++)
	{
		std::string name = players[i].getName();
		players[i] = IRole();
		players[i].setMyIdx(i);
		players[i].setName(name);
		std::cout << players[i].getName() << std::endl;
		players[i].setRoomId(myRoomId);
		std::cout << name << std::endl;
		players[i].setCanListenNow(true);
		players[i].setCanSpeakNow(true);
	}
	//finish();
}

int GameManager::gameCycle() {
	currentState = SPEAKING_STAGE;
	sendToAllInRoom(STAGE_MESSAGE_ID, (char*)&currentState, 4);
	if (!roomCreated) {
		return CLOSED_ROOM_REQUEST_ERROR;
	}
	speakingStage();
	for (int i = 0; i < playersCount; i++)
	{
		players[i].setCanListenNow(true);
	}
	if (!roomCreated) {
		return CLOSED_ROOM_REQUEST_ERROR;
	}
	nightStage();
	if (!roomCreated) {
		return CLOSED_ROOM_REQUEST_ERROR;
	}
	for (int i = 0; i < playersCount; i++)
	{
		players[i].setCanListenNow(true);
	}
	argumentingStage();
	if (!roomCreated) {
		return CLOSED_ROOM_REQUEST_ERROR;
	}
	for (int i = 0; i < playersCount; i++)
	{
		players[i].setCanListenNow(true);
	}
	deathStage();
	if (!roomCreated) {
		return CLOSED_ROOM_REQUEST_ERROR;
	}
	for (int i = 0; i < playersCount; i++)
	{
		players[i].setCanListenNow(true);
	}
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
	int* roles = new int[playersCount+1];
	for (int i = 0; i < playersCount; i++)
	{
		roles[i+1] = players[i].roleIdx();
		//std::cout << roles[i + 1];
	}
	do {
		res = gameCycle();
		roundIndex++;
	} while (res == 0 && roomCreated);
	if (!roomCreated) {
		std::cout << "room closed" << std::endl;
		return;
	}
	roles[0] = res;
	//std::cout << roles[1] << std::endl;
	if (res == -1) {
		
		sendToAllInRoom(RESULTS_MESSAGE_ID, (char*)roles, (playersCount + 1) * 4);
	}
	else {
		//std::cout << *(int*)(roles + 4) << std::endl;
		sendToAllInRoom(RESULTS_MESSAGE_ID, (char*)roles, (playersCount + 1) * 4);
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
	//size == SOUND_SIZE)
	//std::cout << "send" << std::endl;
	if (true) {
		int trueIndex = -1;
		for (int i = 0; i < playersCount; i++)
		{
			if (playersIndexes[i] == index) {
				trueIndex = i;
				break;
			}
		}

		if (trueIndex != -1 && (currentState == WAITING_STAGE || players[trueIndex].canSpeakNow())) {
			//std::cout << "add sound for " << trueIndex << std::endl;
			//players[trueIndex].addSound(data);
			//std::cout << "send1" << std::endl;
			char* newMes = new char[size + 1];
			newMes[0] = (char)trueIndex;
			for (int i = 0; i < size; i++)
			{
				newMes[i+1] = data[i];
			}

			for (int i = 0; i < playersCount; i++)
			{
				//currentSpeaker[i] == trueIndex
				if (players[i].canListenNow() && i != trueIndex) {
					//std::cout << "send2" << std::endl;
					netWorkerSingleton->sendMessage(playersIndexes[i], AUDIO_MESSAGE_ID, newMes, size+1, myRoomId);
				}
			}
			delete[] newMes;
		}
	}
	else {
		std::cout << size << " - incorrect size" << std::endl;
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
		delete[] message;
	}
}

void GameManager::finish()
{
	sendToAllInRoom(EXIT_ROOM_MESSAGE_ID, (char*)"closing", 8);
	for (int i = 0; i < playersCount; i++)
	{
		netWorkerSingleton->throwClient(playersIndexes[i]);
	}
	delete[] key;
	delete[] players;
	delete[] playersIndexes;
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
	int curIdx = 0;
    for(int i = 0; i < playersCount; i++){
		if (!players[i].isInitialized()) {
			//std::cout << "set " << i << std::endl;
			std::string name = players[i].getName();
			players[i] = IRole(roles[curIdx]);
			players[i].setName(name);
			std::cout << players[i].getName() << std::endl;
			players[i].setMyIdx(playersIndexes[i]);
			players[i].setRoomId(myRoomId);
			netWorkerSingleton->sendMessage(playersIndexes[i], ROLE_MESSAGE_ID, (char*)& roles[i], 4, myRoomId);
			curIdx++;
		}
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
			if (currentState == DEATH_STAGE) {
				char* res = new char[8];
				for (int j = 0; j < 4; j++)
				{
					res[j] = ((char*)&voterIdx)[j];
				}
				for (int j = 0; j < 4; j++)
				{
					res[4+j] = ((char*)&playerIdx)[j];
				}
				sendToAllInRoom(MADE_VOTE_MESSAGE, res, 8);
				delete[] res;
			}
			//std::cout << i << " votes for " << playerIdx << std::endl;
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
