#ifndef IROLE_H
#define IROLE_H
#include <iostream>
#include "defines_s.h"

namespace Mafia{
    class IRole{
    public:
		IRole();

		IRole(int roleIdx);

		int lastPlayerVotedIndex();

		void setLastPlayerVotedIndex(int val);

		void setMyIdx(int idx);

		void setName(std::string newName);

		std::string getName();

		char* die(int idx);

		char* hill();

		void setRoomId(int id);

        //returns is role red or black
        bool isRed();

        bool alive();

        int roleIdx();

		bool canSpeakNow();

		bool canListenNow();

		void setCanSpeakNow(bool val);

		void setCanListenNow(bool val);

        //then I maybe will make more functions
		bool isCandidate = false;
		int votes = 0;
		bool answered = false;

		bool getHasSound();

		char* getSound();

		void addSound(char* sound);

		void clearSound();

		bool isInitialized();
	private:
		std::string name = "User";
		char* mySound;
		bool hasSound = false;
		int myIdx = -1;
		int lpvi = -1;
		bool canSpeak = true;
		bool canListen = true;
		int roleId = 0;
		bool red = true;
		bool isAlive = true;
		int myRoomId = 0;
		bool initialized = false;
    };
}

#endif // IROLE_H
