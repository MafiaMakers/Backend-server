#include "include_s.h"
#include "systemfunctions_s.h"
#include "IRole.h"

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
}
