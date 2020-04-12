#include "include_s.h"
#include "IRole.h"
#ifndef SYSTEMFUNCTIONS_H
#define SYSTEMFUNCTIONS_H

namespace Mafia {
    enum OS
    {
        Undef,
        Win,
        Mac
    };

    #ifndef currentOS
        #if defined(__APPLE__) || defined(__MACH__)
            #define currentOS Mac
        #else
            #if defined(_WIN32) || defined(_WIN64)
                #define currentOS Win
            #else
                #define currentOS Undef
            #endif
        #endif
    #endif

    void zeroMemSys(char* arr, int size);

    char* rIdToN(int roleId);

    //fills arr with rolesCount
    int setRolesCount(int playersCount, int* arr);

    void shuffle(int* arr, int size);

    IRole* idToRole(int id);
}
#endif // SYSTEMFUNCTIONS_H
