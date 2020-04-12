

#ifndef NONEROLE_H
#define NONEROLE_H
#include "IRole.h"
namespace Mafia{
    class NoneRole : public IRole
    {
        void nightWork() override;
        bool alive() override;
        bool isRed() override;
        bool checkWin(IRole *players, int size) override;
        int roleIdx() override;
		bool canSpeakNow() override;
		bool canListenNow() override;
		void setCanSpeakNow(bool val) override;
		void setCanListenNow(bool val) override;
    public:
        NoneRole();
    };
}

#endif // NONEROLE_H
