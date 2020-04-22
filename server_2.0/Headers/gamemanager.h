

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "include_s.h"
#include "IRole.h"
#include "systemfunctions_s.h"

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
        void deathStage(int deathRound = 0);
        void resultsStage();
        void finish();
		void fullGame();
		void vote(int voterIdx, int playerIdx);
		void sendToAllAlive(short messageId, char* message, int mesLen);
        ~GameManager();
		bool canSpeak(int index);
		bool canListen(int index);

		bool checkEmpty();

		void sendVideo(char* data, int size, int index);

		void sendAudio(char* data, int size, int index);

		void setMyRoomId(int id);

		void setupRoles(int* rolesCount);

		void nextStage();

		void answer(int index);

		bool isInitialized();

		bool isOpened();

		char* getKey();

		int getAdminIdx();

		int getPlayerByIndex(int index);

		char* initRoom();


		int getPlayersCount();

		void sendToAllInRoom(short messageId, char* message, int mesLen);

		bool isPlayerIn(int index);

		void setAdmin(int index);

		int addPlayer(int index);
		//static bool objectInitialized = 0;
    private:
		void _findNewAdmin();
		void _freePlayers();
		int _checkWin();
        //fills arr with rolesCount
        int _setRolesCount(int* arr);
        int _getFather();
        int* _shuffleRoles(int* arr);
        void _setRoles(int* roles);
		void _sendCandidates();

        int currentState = WAITING_STAGE;
        IRole* players = new IRole[CLIENTS_MAX_COUNT];
        int playersCount = 0;
		int roundIndex = 0;
		bool gonext = false;
		bool roomCreated = false;
		bool roomOpened = false;
		int *playersIndexes = new int[PLAYERS_MAX_COUNT];
		int adminIdx = 0;
		int myRoomId = 0;
		char *key = new char[KEY_SIZE];
    };
}


#endif // GAMEMANAGER_H
