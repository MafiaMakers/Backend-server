#include "include_s.h"
#include "systemfunctions_s.h"
#include "networker_s.h"
#include <unistd.h>
#include <errno.h>
#include <chrono>



using namespace Mafia;

int main()
{
    std::cout << "Hello I'm server!" << std::endl;
    NetWorker myNetWorker = NetWorker();

    std::cout << myNetWorker.initServer() << std::endl;
    std::thread receiveTh(&NetWorker::processMessages, &myNetWorker);
    receiveTh.detach();

    char * ans = new char[BUF_SIZE];
    zeroMemSys(ans, BUF_SIZE);
    std::cin >> ans;
    if(ans[0] == 'c'){
        std::cout << "initializing game" << std::endl;
        myNetWorker.initGame();
    }

    do{
        zeroMemSys(ans, BUF_SIZE);
        std::cin >> ans;
        if(ans[0] == 'f'){
            std::cout << "finishing" << std::endl;
            myNetWorker.closeRoom();
        }

    }while(ans[0] != 'f');

    if(receiveTh.joinable()){
        receiveTh.join();
    }

    myNetWorker.finish();
}
