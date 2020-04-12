#include "IRole.h"
#include "civillian.h"
#include "include_s.h"
#include "systemfunctions_s.h"

namespace Mafia{
    Civillian::Civillian(){}

    /*char* Civillian::getName(){
        return rIdToN(roleId);
    }*/

    int Civillian::nightWork(){
        return (0);
    }

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


}

