#include "IRole.h"
#include "include_s.h"
#ifndef CIVILLIAN_H
#define CIVILLIAN_H

namespace Mafia{
    class Civillian : public IRole
    {
    public:
        Civillian();
        //char * getName() override;
        int nightWork() override;
        bool checkWin(IRole* players, int size) override;
        bool isRed() override;
        bool alive() override;
    protected:
        static const int roleId = CIVILLIAN_ROLE;
        static const bool red = true;
        bool isAlive = true;
    };
}

#endif // CIVILLIAN_H
