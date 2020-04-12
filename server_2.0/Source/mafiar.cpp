#include "mafiar.h"
#include "include_s.h"
#include "IRole.h"
#include "systemfunctions_s.h"

namespace Mafia{
	MafiaR::MafiaR(){}

    bool MafiaR::alive(){
        return isAlive;
    }

    void MafiaR::nightWork(){}

    int MafiaR::roleIdx(){return MAFIA_ROLE;}

    bool MafiaR::checkWin(IRole* players, int size){
        int diff = 0;
        for(int i = 0; i < size; i++){
            if(players[i].isRed()){
                diff++;
            } else{
                diff--;
            }
        }
        return(diff <= 0);
    }


    bool MafiaR::isRed(){
        return red;
    }

	bool MafiaR::canListenNow() {
		return canListen;
	}

	bool MafiaR::canSpeakNow() {
		return canSpeak;
	}

	void MafiaR::setCanSpeakNow(bool val) {
		canSpeak = val;
	}

	void MafiaR::setCanListenNow(bool val) {
		canListen = val;
	}

}
