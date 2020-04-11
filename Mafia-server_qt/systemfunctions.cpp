#include "include.h"
#include "systemfunctions.h"
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

}
