#include "include_s.h"
#include "systemfunctions_s.h"
#include "networker_s.h"

using namespace Mafia;

int main()
{
    std::cout << "Hello I'm server!" << std::endl;
	NetWorker * myNetWorker = new NetWorker();
	myNetWorker->initServer();
	std::thread recTh(&NetWorker::processMessages, netWorkerSingleton);
	std::thread checkTh(&NetWorker::checkConnections, netWorkerSingleton);

	recTh.detach();
	checkTh.detach();

	std::string answer = "";
	while (true){
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
