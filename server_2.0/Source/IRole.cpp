#include "IRole.h"
#include <iostream>
#pragma warning(disable:4996)
using namespace Mafia;

IRole::IRole(){}


char* IRole::die(int idx) {
	isAlive = false;
	char* res = new char[5];
	res[0] = (char)isAlive;
	for (int i = 0; i < 4; i++)
	{
		res[i + 1] = ((char*)& idx)[i];
	}
	return res;
}

bool IRole::getHasSound() {
	return hasSound;
}

void IRole::addSound(char* sound) {
	if (hasSound) {
		delete[] mySound;
	}
	hasSound = true;
	mySound = new char[SOUND_SIZE];
	for (int i = 0; i < SOUND_SIZE; i++)
	{
		mySound[i] = sound[i];
	}
	//this->mySound = sound;
}

void IRole::clearSound() {
	hasSound = false;
}

char* IRole::getSound() {
	return mySound;
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
	initialized = true;
	std::cout << "now initialized!" << std::endl;
}

bool IRole::isInitialized() {
	return initialized;
}

void IRole::setName(std::string newName) {
	this->name = newName;
}

std::string IRole::getName() {
	return this->name;
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
