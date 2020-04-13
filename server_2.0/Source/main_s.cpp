#include "include_s.h"
#include "systemfunctions_s.h"
#include "gamemanager.h"

using namespace Mafia;

int main()
{
    std::cout << "Hello I'm server!" << std::endl;
    //NetWorker myNetWorker = NetWorker();
	std::string answer = "";
    GameManager* GM = new GameManager();
    do{

        //zeroMemSys(ans, BUF_SIZE);
        std::cin >> answer;
        if(answer == "start"){
            std::cout << "starting" << std::endl;
			GM->fullGame();
        }

    }while(true);

	GM->finish();
}
