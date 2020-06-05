#ifndef MAINSERVERNETWORKER_H
#define MAINSERVERNETWORKER_H
#include <QUdpSocket>
#include "crypto.h"
#include "messageTypes.h"

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
         * \brief Отправления сообщения
         * \param message Сообщение для отправки. Должно быть инициализированно во всех полях.
         * Если id сообщения не фиксированно, то следует в поле id указать 0
         */
        MessageIdType send_message(Message message);
    signals:
        /*!
         * \brief Сигнал, вызываемый при получении ответа на какой-либо сетевой запрос. К нему автоматически коннектятся все объекты запросов
         * \param message Пришедшее сообщение с ответом
         */
        void request_answer(Message message);
        /*!
         * \brief Сигнал, вызываемый при получении сообщения от api субсервера
         * \param message Пришедшее сообщение
         */
        void on_subserver_api_message_received(Message message);
    private slots:
        /*!
         * \brief слот для получения сообщения. Вызывается автоматически
         */
        void receive_message();
        /*!
         * \brief Функция вывода всей информации о сообщении
         * \param mes Сообщение
         */
        void show_message(Message mes);

    private:
        /*!
         * \brief Отправка сообщения
         * \param data массив символов, сообщение
         * \param size размер сообщения в байтах
         * \param client клиент, которому необходимо послать сообщение
         */
        void _send_message(char* data, int size, QHostAddress client, int port);
        /*!
         * \brief Обработка сообщения
         * \param message Сообщение
         */
        void _process_message(Message message);
        /*!
         * \brief Проверка и отправка всех неподтвержденных сообщений
         */
        void _resend_not_confirmed_messages();
        /*!
         * \brief Отправка сообщения с задержкой
         * \param message Сообщение
         * \param delay Задержка (мс)
         */
        void late_send_message(Message message, int delay);

        //! сокет сервера
        QUdpSocket* socket;
        //! порт сервера
        int myPort;
        //! Сообщения, ожидающие подтверждения
        QList<Message> waitingForConfirmation;
        //! id сообщения, которое на данный момент максимальное
        MessageIdType currentMaxId = (MessageIdType)0;
        //! Множество типов сообщений, которым необходимо подтверждение
        static const QSet<MessageTypeType> needConfirmation;
        //! Время в мс через которое проводится повторная отправка сообщений, которые не получили подтверждение
        static const int timeToResend;
        //! Максимальное количество повторных отправок
        static const int maxResendCount;
    };
}

#endif // MAINSERVERNETWORKER_H
