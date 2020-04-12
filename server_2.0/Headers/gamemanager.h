

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "include_s.h"
#include "RolesInclude.h"
#include "systemfunctions_s.h"
#include "networker_s.h"

namespace Mafia{
    //Singleton
    class GameManager
    {
    public:
        GameManager();
        bool gameCycle();
        void initGame();
        void speakingStage();
        void nightStage();
        void argumentingStage();
        void deathStage();
        void resultsStage();
        bool checkFinish();
        void finish();
        ~GameManager();
		//static bool objectInitialized = 0;
    private:
        //fills arr with rolesCount
        int _setRolesCount(int* arr);
        int _getFather();
        int* _shuffleRoles(int* arr);
        void _setRoles(int* roles);
		void _joinThreads();
        int currentState = WAITING_STAGE;
        IRole** players = new IRole*[CLIENTS_MAX_COUNT];
        std::thread mesReceivingThread;
		std::thread checkConThread;
        int playersCount = 0;
    };
}


#endif // GAMEMANAGER_H
