#ifndef MAINSERVERNETWORKER_H
#define MAINSERVERNETWORKER_H
#include <QUdpSocket>
#include "crypto.h"
#include "System/functions.h"
#include "message.h"

namespace Mafia {
    //! \brief Пространство имен, отвечающее за передачу данных по интернету
    namespace Network {
        /*!
         * \brief Основной класс отправки и получения сообщений для основного сервера
         */
        class MainServerNetworker : public QObject
        {
        Q_OBJECT
        public:
			MainServerNetworker(QObject* parent = nullptr);

            /*!
             * \brief Конструктор, инициализирует сервер на указанном порте
             * \param port порт, на котором инициализировать сервер
             */
            MainServerNetworker(int port);

			~MainServerNetworker();

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

            /*!
             * \brief Сигнал, вызываемый при получении сообщения такого типа,
             * который не предусмотрен стандартным обработчиком MainServerNetworker'a
             * \param message Все данные сообщения
             */
            void message_received(Message message);
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
             * \brief Функция, добавляющая в список сообщений, которые ожидают сборки по частям, нового сообщения
             * \param baseMessage Первое сообщение партии, которое берется за основу
             */
            void _add_empty_message(Message baseMessage);

            /*!
             * \brief Функция, конструирующая цельное сообщение из кусочков
             * \param id id сообщения, которое следует собрать
             * \return Сообщение со всеми заполненными полями
             */
            Message _construct_whole_message(MessageIdType id);

            /*!
             * \brief Функция, подтверждающая получение сообщения с указанным id (вызывается при получении сообщения-подтверждения)
             * \param id id сообщения, получение которого следует подтвердить
             */
            void _confirm_message(MessageIdType id);

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
            /*!
             * \brief Функция, которая проверяет, получены ли все части от сообщения
             * \param id id сообщения, которое надо проверить на целостность
             * \return true, если все части получены и сообщение готово к обработке, иначе - false
             */
            bool check_message_full(MessageIdType id);
            /*!
             * \brief Получение индекса сообщения в списке сообщений, ожидающих заполнения
             * \param id id сообщения
             * \return Индекс этого сообщения в списке \ref Mafia::Network::waitingToFillMessages
             */
            int get_list_index(MessageIdType id);
            /*!
             * \brief Добавление сообщения (из сети). Функция собирает сообщение по частям.
             * Получив сообщение она добавляет его как часть уже существующего сообщения или создает новое
             * \param message Сообщение, полученное по сети
             */
            void add_received_message(Message message);

            /*!
             * \brief Проверка на то, совпадают ли данные сообщений, которые должны быть частями одного сообщения.
             * Если серверу сначала пришло сообщение с id = 1, type = 3, от IP = 165.145.11.4, то все сообщения далее,
             * которые являются частями того же сообщения, что и первое (имеют тот же id), должны иметь тот же тип,
             * прийти от того же IP, иметь то же кол-во частей. Иначе они будут считаться неверными данными
             * \param message Сообщение, пришедшее по сети
             * \return true, если все ОК, false, если есть хоть одно несовпадение
             */
            bool message_matches(Message message);

            //! сокет сервера
            QUdpSocket* socket;
            //! порт сервера
            int myPort;
            //! Сообщения, ожидающие подтверждения
            MafiaList<Message> waitingForConfirmation;
            //! id сообщения, которое на данный момент максимальное
            MessageIdType currentMaxId = (MessageIdType)0;
            //! \brief Список всех сообщений, ожидающих заполнения.
            //! Как вообще работает эта система: при отправке сообщения оно разбивается на части, каждая из которых занимает не больше
            //! \ref Mafia::Network::MainServerNetworker::MAX_MESSAGE_SIZE. Далее эти сообщения-части отправляются по одному, чтобы не
            //! перегружать роутер (некоторые роутеры блокируют сообщения больше 1Кб).
            //! Далее, когда кто-то получает сообщение, он сначала получает только его часть, затем еще и еще часть...
            //! В этом списке хранятся все части сообщений (1 элемент списка - это список со всеми частями этого сообщения.)
            //! В каждой части сообщения хранится информация о том, какой номер у этой части и сколько частей всего
            //! Каждый элемент этого списка - это список размером в точности количество частей этого сообщения, причем те части, которые еще
            //! не получены, заполнены нулевыми сообщениями (\ref Mafia::Network::Message::Message())
            //! Как только получены все части сообщения, из них конструируется большое сообщение, а части удаляются
            MafiaList<MafiaList<Message>> waitingToFillMessages;
            //! \brief Множество типов сообщений, которым необходимо подтверждение
            static const QSet<MessageType> needConfirmation;
            //! \brief Время в мс через которое проводится повторная отправка сообщений, которые не получили подтверждение
            static const int TIME_TO_RESEND;
            //! \brief Максимальное количество повторных отправок
            static const int MAX_RESEND_COUNT;
            //! \brief Максимальная длина данных одного сообщения (в байтах)
            static const int MAX_MESSAGE_SIZE = 512;
        };
    }
}

#endif // MAINSERVERNETWORKER_H
