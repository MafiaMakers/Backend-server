#include "include_s.h"
#include "systemfunctions_s.h"
#include "networker_s.h"
#include <unistd.h>
#include <errno.h>



using namespace Mafia;

int main()
{
    std::cout << "Hello server!" << std::endl;
    NetWorker myNetWorker = NetWorker();

    std::cout << myNetWorker.initServer() << std::endl;
    std::thread receiveTh(&NetWorker::receiveMessage, &myNetWorker);
    receiveTh.detach();
    //std::cout << receiveTh.joinable() << std::endl;

    char * ans = new char[BUF_SIZE];
    zeroMemSys(ans, BUF_SIZE);
    std::cin >> ans;
    if(ans[0] == 'c'){
        std::cout << "closing" << std::endl;
        myNetWorker.closeRoom();
    }

    if(receiveTh.joinable()){
        receiveTh.join();
    }

    myNetWorker.finish();
}
