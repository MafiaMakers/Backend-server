/* Developed by Никита Андрусов
 * Если у вас есть вопросы по коду, пишите vk.com/n_andrusov
 * Если вам что-то не нравится, не убивайте пж...
 * */

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

    MainServerManager* myManager = new MainServerManager(argc, argv);

    return a.exec();
}
