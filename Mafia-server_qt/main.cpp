#include "include.h"
#include "systemfunctions.h"
#include "networker.h"

using namespace Mafia;

int main()
{
    NetWorker myNetWorker = NetWorker();
    std::cout << myNetWorker.initServer("127.0.0.1") << std::endl << "Server successfully initialized" << std::endl;
    return 0;
}
