#include "civillian.h"
#include "defines_s.h"
namespace Mafia{
    Civillian::Civillian(){}

    void Civillian::nightWork(){}

    bool Civillian::checkWin(IRole* players, int size){
        int diff = 0;
        for(int i = 0; i < size; i++){
            if(players[i].isRed()){
                diff++;
            } else{
                diff--;
            }
        }
        return(diff > 0);
    }

    bool Civillian::alive(){
        return isAlive;
    }

    bool Civillian::isRed(){
        return red;
    }

	int Civillian::roleIdx() {
		return CIVILLIAN_ROLE;
	}

	bool Civillian::canListenNow() {
		return canListen;
	}

	bool Civillian::canSpeakNow() {
		return canSpeak;
	}

	void Civillian::setCanSpeakNow(bool val) {
		canSpeak = val;
	}

	void Civillian::setCanListenNow(bool val) {
		canListen = val;
	}

}

