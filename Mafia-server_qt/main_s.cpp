#include "include_s.h"
#include "systemfunctions_s.h"
#include "networker_s.h"
#include <unistd.h>
#include <errno.h>



using namespace Mafia;

int main()
{
    std::cout << "Hello server!" << std::endl;;
    NetWorker myNetWorker = NetWorker();
    std::cout << myNetWorker.initServer() << std::endl;
    if(myNetWorker.receiveMessage() == 0){
    } else{
        std::cout << "w";
    }
    myNetWorker.finish();
}
