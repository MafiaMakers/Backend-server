#include "nonerole.h"

namespace Mafia {
    NoneRole::NoneRole(){}
    bool NoneRole::alive(){return false;}
    bool NoneRole::isRed(){return true;}
    int NoneRole::roleIdx(){return -1;}
    //char* NoneRole::getName(){return "None";}
    bool NoneRole::checkWin(IRole *players, int size){return false;}
    void NoneRole::nightWork(){}

	bool NoneRole::canListenNow() {return false;}

	bool NoneRole::canSpeakNow() {return false;}

	void NoneRole::setCanSpeakNow(bool val) {}

	void NoneRole::setCanListenNow(bool val) {}


}
