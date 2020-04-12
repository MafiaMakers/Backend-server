#include "nonerole.h"

namespace Mafia {
    NoneRole::NoneRole(){}
    bool NoneRole::alive(){return false;}
    bool NoneRole::isRed(){return true;}
    //char* NoneRole::getName(){return "None";}
    bool NoneRole::checkWin(IRole *players, int size){return false;}
    int NoneRole::nightWork(){return 0;}
}
