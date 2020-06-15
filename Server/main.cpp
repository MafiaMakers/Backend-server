#include <QCoreApplication>
#include "mainservermanager.h"
#include "System/runprogram.h"
#include "subserverobject.h"
#include "System/SHA_256.h"
#include "Database/databaseworker.h"
#include "Database/userdatabaseworker.h"
using namespace Mafia;
/*!
 * \brief Это пример использования этой системы классов. Пока это просто заглушка, можно сказать.
 * \return код ошибки
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Database::DatabaseWorker myDBWorker = Database::DatabaseWorker();

    //run_app(String("D:\\Dropbox\\Dropbox\\Nikita\\Programs_1\\c++\\Mafia\\ClientSumm\\ClientSumm\\Client-summ\\release\\ForLibsTest.exe"), String("ForLibsTest.exe"), String(""));

    /*MainServerNetworker * nsnw = new MainServerNetworker(10000);

    //MainServerNetworker * ssnw = new MainServerNetworker(10001);

    Message mes1 = Message();
    mes1.id = 1;
    mes1.data = (char*)"This message doesn't need confirmation";
    mes1.size = 39;
    mes1.type = NO_CINFIRM_TEXT_MESSAGE_TYPE;
    mes1.client.ip = QHostAddress("192.168.1.66").toIPv4Address();
    mes1.client.port = 10000;

    Crypto::setKey("HAHA_UNDECRYPTABLE_KEY");

    nsnw->send_message(mes1);

    Message mes2 = Message();
    mes2.id = -1;
    mes2.data = (char*)"This message needs confirmation";
    mes2.size = 32;
    mes2.type = TEXT_MESSAGE_TYPE;
    mes2.client.ip = QHostAddress("192.168.1.66").toIPv4Address();
    mes2.client.port = 10000;

    nsnw->send_message(mes2);*/

    MainServerManager* myManager = new MainServerManager();

    return a.exec();
}
