#include <QCoreApplication>
#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include <thread>
#include <chrono>


using namespace Mafia;
/*!
 * \brief Это пример использования этой системы классов. Пока это просто заглушка, можно сказать.
 * \return код ошибки
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainServerNetworker * nsnw = new MainServerNetworker(10000);

    MainServerNetworker * ssnw = new MainServerNetworker(10001);

    Message mes1;
    mes1.id = 179;
    mes1.data = (char*)"Hello world!";
    mes1.size = 13;
    mes1.type = 1;
    String myMessage;
    try {
        myMessage = Crypto::wrap_message(mes1);
    } catch (Mafia::Exception* exception) {
        exception->show();
    }

    ssnw->send_message(myMessage.data, myMessage.size, QHostAddress("192.168.1.66"));
    return a.exec();
}
