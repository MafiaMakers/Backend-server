#include "IRole.h"
using namespace Mafia;

IRole::IRole(){}


char* IRole::die() {
	isAlive = false;
	char* res = new char[5];
	res[0] = (char)isAlive;
	for (int i = 0; i < 4; i++)
	{
		res[i + 1] = ((char*)& myIdx)[i];
	}
	return res;
}

char* IRole::hill() {
	isAlive = true;
	char* res = new char[5];
	res[0] = (char)isAlive;
	for (int i = 0; i < 4; i++)
	{
		res[i + 1] = ((char*)& myIdx)[i];
	}
	return res;
}

IRole::IRole(int rIdx) {
	roleId = rIdx;
	if (rIdx == MAFIA_ROLE) {
		red = false;
	}
	else {
		red = true;
	}
}

bool IRole::alive() {
	return isAlive;
}

void IRole::setRoomId(int id) {
	myRoomId = id;
}

int IRole::roleIdx() { return roleId; }


bool IRole::isRed() {
	return red;
}

bool IRole::canListenNow() {
	return canListen;
}

bool IRole::canSpeakNow() {
	return canSpeak;
}


void IRole::setCanListenNow(bool val) {
	canListen = val;
}

int IRole::lastPlayerVotedIndex() {
	return lpvi;
}

void IRole::setLastPlayerVotedIndex(int val) {
	lpvi = val;
}

void IRole::setMyIdx(int val) {
	myIdx = val;
}
