#include "IRole.h"

#ifndef NONEROLE_H
#define NONEROLE_H

namespace Mafia{
    class NoneRole : public IRole
    {
        int nightWork() override;
        bool alive() override;
        bool isRed() override;
        bool checkWin(IRole *players, int size) override;
        //char * getName() override;

    public:
        NoneRole();
    };
}

#endif // NONEROLE_H
