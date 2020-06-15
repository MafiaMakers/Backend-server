#include "game_s.h"
#include "Exceptions/gameplayexception.h"
using namespace Mafia;
using namespace Gameplay;


Mafia::Gameplay::GamePersonalResult Mafia::Gameplay::result_by_role_and_res(Mafia::Gameplay::Role role, Mafia::Gameplay::GameResult res)
{
    switch (res) {
    case GameResult_None:{
        return GamePersonalResult_None;
    }

    case GameResult_MafiaWins:{
        switch (role) {
        case Role_Mafia:{
            return GamePersonalResult_Victory;
        }
        case Role_None:{
            return GamePersonalResult_None;
        }
        default:{
            return GamePersonalResult_Defeat;
        }
        }
        break;
    }
    case GameResult_CiviliansWin:{
        switch(role){
        case Role_Mafia:{
            return GamePersonalResult_Defeat;
        }
        case Role_None:{
            return GamePersonalResult_None;
        }
        default:{
            return GamePersonalResult_Victory;
        }
        }
    }
    default:{
        throw new Exceptions::GameplayException(System::String("Role id not found"), Exceptions::GameplayExceptionId_UnknownRole);
        return GamePersonalResult_None;
    }
    }
}
