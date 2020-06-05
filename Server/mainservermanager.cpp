#include "mainservermanager.h"
#include "System/runprogram.h"
#include "subserverobject.h"
using namespace Mafia;


MainServerManager::MainServerManager(QObject* parent) : QObject(parent)
{
    networker = new MainServerNetworker(10000);
    Crypto::setKey("HaHA_UnDeCrYptAbLe_keY");
    NetworkRequest * myTestRequest = 0;
    try {
        myTestRequest = new NetworkRequest(networker,
                                              Message(ABSTRACT_REQUEST_MESSAGE_TYPE,
                                                      (char*)"Some ask",
                                                      9,
                                                Client(QHostAddress("192.168.1.66").toIPv4Address(),
                                                       10000)));



    } catch (Exception* exception) {
        exception->show();
    }

    SubserverObject* subserver = new SubserverObject(networker, 100001,
                        String("D:\\Dropbox\\Dropbox\\Nikita\\Programs_1\\c++\\Mafia\\ClientSumm\\ClientSumm\\Client-summ\\release\\ForLibsTest.exe"),
                        String("ForLibsTest.exe"));

    std::thread getDataThread(&MainServerManager::_get_data_from_request, this, myTestRequest);
    getDataThread.detach();

}

void MainServerManager::_get_data_from_request(NetworkRequest *req)
{
    if(req != 0){
        try {
            req->sleep_untill_finished();
            std::cout << "The answer is " << req->get_result<int>() << std::endl;
        } catch (Exception* exception) {
            exception->show();
        }
    }
}
