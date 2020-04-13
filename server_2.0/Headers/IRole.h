#ifndef IROLE_H
#define IROLE_H
#include "defines_s.h"

namespace Mafia{
    class IRole{
    public:
		IRole();

		IRole(int roleIdx);

		int lastPlayerVotedIndex();

		void setLastPlayerVotedIndex(int val);

		void setMyIdx(int idx);

		void die();

		void hill();

        //returns is role red or black
        bool isRed();

        bool alive();

        int roleIdx();

		bool canSpeakNow();

		bool canListenNow();

		void setCanSpeakNow(bool val);

		void setCanListenNow(bool val);
        //then I maybe will make more functions

	private:
		int myIdx = -1;
		int lpvi = -1;
		bool canSpeak = true;
		bool canListen = true;
		int roleId = 0;
		bool red = true;
		bool isAlive = true;
    };
}

#endif // IROLE_H
