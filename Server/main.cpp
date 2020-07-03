#define DEBUG_MODE

#include <QCoreApplication>
#include <Mafia>
/*!
 * \brief Это пример использования этой системы классов. Пока это просто заглушка, можно сказать.
 * \return код ошибки
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Database::DatabaseWorker myDBWorker = Database::DatabaseWorker();

    MainServerManager* myManager = new MainServerManager(argc, argv);

    myManager->_add_user_test();

    return a.exec();
}
