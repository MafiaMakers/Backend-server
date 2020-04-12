#ifndef IROLE_H
#define IROLE_H

namespace Mafia{
    class IRole{
    public:
        //all the nighwork
        virtual int nightWork() = 0;
        //in the end of the game checks if this player won
        virtual bool checkWin(IRole* players, int size) = 0;
        //returns name of this role
        //virtual char* getName();

        //returns is role red or black
        virtual bool isRed() = 0;

        virtual bool alive() = 0;
        //then I maybe will make more functions
    };
}

#endif // IROLE_H
