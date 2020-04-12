
#ifndef CIVILLIAN_H
#define CIVILLIAN_H
#include "IRole.h"
#include "defines_s.h"


namespace Mafia{
    class Civillian : public IRole
    {
    public:
        Civillian();
        //char * getName() override;
        //int nightWork(NetWorker* netWorker) override;
        void nightWork() override;
        //int nightWork(int *netWorker) override;
        bool checkWin(IRole* players, int size) override;
        bool isRed() override;
        bool alive() override;
        int roleIdx() override;
		bool canSpeakNow() override;
		bool canListenNow() override;
		void setCanSpeakNow(bool val) override;
		void setCanListenNow(bool val) override;
    protected:
		bool canSpeak = true;
		bool canListen = true;
        static const int roleId = CIVILLIAN_ROLE;
        static const bool red = true;
        bool isAlive = true;
    };
}

#endif // CIVILLIAN_H
