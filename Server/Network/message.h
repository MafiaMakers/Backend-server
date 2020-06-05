#ifndef MESSAGE_H
#define MESSAGE_H

/*!
 * \brief Тип, в котором хранится id сообщения
 */
typedef int MessageIdType;
/*!
 * \brief Тип, в котором хранится тип сообщения
 */
typedef short MessageTypeType;
/*!
 * \brief Тип, в котором передаются символы (может быть заменен на wchar_t или qchar16 или еще на что-нибудь похожее)
 */
typedef char SymbolType;
/*!
 * \brief Тип, в котором хранится контрольная сумма (может быть заменен на long)
 */
typedef int ControlSumType;


namespace Mafia {

    /*!
     * \brief Клиент, информация о нем нужна для подтверждения получения сообщения
     */
    struct Client{
        /*!
         * \brief Нулевой конструктор
         */
        Client(){
            ip = 0;
            port = 0;
        }
        /*!
         * \brief Конструктор, задающий все значения
         * \param ip ip
         * \param port порт
         */
        Client(int ip, int port){
            this->ip = ip;
            this->port = port;
        }

        /*!
         * \brief Оператор сравнения
         * \param a второй объект клиент
         * \return true, если это один и тот же клиент, иначе false
         */
        bool operator == (Client a){
            return(a.ip == this->ip && a.port == this->port);
        }

        //! \brief ip отправителя сообщения
        int ip;
        //! \brief порт, с которого было отправлено сообщение
        int port;
    };

    /*!
     * \brief Основная структура для сообщений
     */
    struct Message{
        /*!
         * \brief Нулевой конструктор
         */
        Message(){
            id = MessageIdType();
            type = MessageTypeType();
            data = 0;
            size = 0;
            client = Client();
        }
        /*!
         * \brief Конструктор, задающий все параметры
         * \param id id сообщения
         * \param type тип сообщения
         * \param data данные
         * \param size размер массива данных
         * \param client клиент
         */
        Message(MessageTypeType type, SymbolType* data, int size, Client client, MessageIdType id = 0){
            this->id = id;
            this->data = data;
            this->size = size;
            this->type = type;
            this->client = client;
        }
        //! id сообщения (используется для проверки повторных отправок и прочих сбоев)
        MessageIdType id;
        //! тип сообщения
        MessageTypeType type;
         //! данные сообщения
        SymbolType* data;
        //! размер данных сообщения
        int size;
        //! Клиент-отправитель
        Client client;
    };

}

#endif // MESSAGE_H
