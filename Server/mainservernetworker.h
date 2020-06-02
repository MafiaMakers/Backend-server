#ifndef MAINSERVERNETWORKER_H
#define MAINSERVERNETWORKER_H
#include <QUdpSocket>
#include "crypto.h"
namespace Mafia {
/*!
     * \brief Основной класс отправки и получения сообщений для основного сервера
     */
    class MainServerNetworker : public QObject
    {
    Q_OBJECT
    public:
        /*!
         * \brief Конструктор, инициализирует сервер на указанном порте
         * \param port порт, на котором инициализировать сервер
         */
        MainServerNetworker(int port);
        /*!
         * \brief Отправка сообщения
         * \param data массив символов, сообщение
         * \param size размер сообщения в байтах
         * \param client клиент, которому необходимо послать сообщение
         */
        void send_message(char* data, int size, QHostAddress client);
    public slots:
        /*!
         * \brief слот для получения сообщения. Вызывается автоматически
         */
        void receive_message();

    private:
        //! сокет сервера
        QUdpSocket* socket;
        //! порт сервера
        int myPort;

    };
}

#endif // MAINSERVERNETWORKER_H
