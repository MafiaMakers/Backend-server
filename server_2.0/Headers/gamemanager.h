

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "include_s.h"
#include "IRole.h"
#include "systemfunctions_s.h"
#include "networker_s.h"

namespace Mafia{
    //Singleton
    class GameManager
    {
    public:
        GameManager();
        int gameCycle();
        void initGame();
        void speakingStage();
        void nightStage();
        void argumentingStage();
        void deathStage();
        void resultsStage();
        void finish();
		void fullGame();
		void vote(int voterIdx, int playerIdx);
		void sendToAllAlive(short messageId, char* message, int mesLen);
        ~GameManager();
		bool canSpeak(int index);
		bool canListen(int index);

		void setupRoles(int* rolesCount);

		void nextStage();

		void answer(int index);

		//static bool objectInitialized = 0;
    private:
		int _checkWin();
        //fills arr with rolesCount
        int _setRolesCount(int* arr);
        int _getFather();
        int* _shuffleRoles(int* arr);
        void _setRoles(int* roles);
		void _joinThreads();
        int currentState = WAITING_STAGE;
        IRole* players = new IRole[CLIENTS_MAX_COUNT];
        std::thread mesReceivingThread;
		std::thread checkConThread;
        int playersCount = 0;
		int roundIndex = 0;
		bool gonext = false;
    };
}


#endif // GAMEMANAGER_H
