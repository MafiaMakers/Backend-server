#include "include_s.h"
#include "systemfunctions_s.h"
#include "IRole.h"
#include <ctime>


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
        case CIVILIAN_ROLE: return (char*)"Civillian";
        default: return (char*)"None";
        }
    }

	int random() {
		return((int)time(NULL) * 179 % rand());

	}
}
