

#ifndef MAFIAR_H
#define MAFIAR_H
#include "defines_s.h"
#include "IRole.h"
namespace Mafia{
    class MafiaR : public IRole
    {
    public:
        MafiaR();
        void nightWork() override;
        bool checkWin(IRole* players, int size) override;
        bool isRed() override;
        bool alive() override;
        int roleIdx() override;
        bool isFather = false;
		bool canSpeakNow() override;
		bool canListenNow() override;
		void setCanSpeakNow(bool val) override;
		void setCanListenNow(bool val) override;
    private:
		bool canSpeak = true;
		bool canListen = true;
        static const int roleId = MAFIA_ROLE;
        static const bool red = false;
        bool isAlive = true;
    };

}

#endif // MAFIAR_H
