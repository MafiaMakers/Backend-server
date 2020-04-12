#include "include_s.h"
#include "systemfunctions_s.h"
#include "IRole.h"
#include "RolesInclude.h"

namespace Mafia {
    void zeroMemSys(char* arr, int size) {
        switch (currentOS) {
        case Win:
            ZeroMemory(arr, size);
            break;
        case Mac:
            memset(arr, 0, size);
            break;
        default:
            break;
        }
    }

    char * rIdToN(int roleId){
        switch(roleId){
        case CIVILLIAN_ROLE: return (char*)"Civillian";
        default: return (char*)"None";
        }
    }

    int setRolesCount(int playersCount, int* arr){
        if(playersCount < 1){
            return -1;
        }
        if(playersCount < 4){
            arr[CIVILLIAN_ROLE] = playersCount;
            for(int i = 1; i < MAX_ROLE_ID; i++){
                arr[i] = 0;
            }
            return 0;
        }
        if(playersCount < 7){
            arr[CIVILLIAN_ROLE] = 6;
            arr[MAFIA_ROLE] = 1;
            for(int i = 2; i < MAX_ROLE_ID; i++){
                arr[i] = 0;
            }
            return 0;
        }

    }

    void shuffle(int* arr, int size){
        for(int i = 0; i < size; i++){
            int f_ind = (unsigned int)rand() % size;
            int s_ind = (unsigned int)rand() % size;

            int tmp = arr[f_ind];
            arr[f_ind] = arr[s_ind];
            arr[s_ind] = tmp;
        }
    }

    IRole* idToRole(int id){
        switch (id) {
        case CIVILLIAN_ROLE: return new Civillian();
        default: return new NoneRole();
        }
    }

}
