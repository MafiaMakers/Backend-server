#include "IRole.h"

using namespace Mafia;

IRole::IRole(){}

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
