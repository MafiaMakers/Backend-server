#ifndef MESSAGE_H
#define MESSAGE_H
#include "messageTypes.h"
namespace Mafia {
    namespace Network {
        /*!
         * \brief Тип, в котором хранится id сообщения
         */
        typedef int MessageIdType;
        /*!
         * \brief Тип, в котором передаются символы (может быть заменен на wchar_t или qchar16 или еще на что-нибудь похожее)
         */
        typedef char SymbolType;
        /*!
         * \brief Тип, в котором хранится контрольная сумма (может быть заменен на long)
         */
        typedef int ControlSumType;
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

            /*!
             * \brief Тоже оператор сравнения, но только обратный
             * \param a второй объект клиент
             * \return true, если объекты не совпадают хоть в чем-то (ip или port), false, если объекты абсолютно идентичны
             */
            bool operator != (Client a){
                return !(*this == a);
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
                type = MessageType();
                data = 0;
                size = 0;
                partIndex = 0;
                partsCount = 1;
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
            Message(MessageType type, SymbolType* data, int size, Client client, MessageIdType id = 0, int partsCount = 1, int partIndex = 0){
                this->id = id;
                this->data = data;
                this->size = size;
                this->type = type;
                this->client = client;
                this->partIndex = partIndex;
                this->partsCount = partsCount;
            }

            /*!
             * \brief Оператор присваивания
             * \param mes другое сообщение
             * \return Полученное присвоенное сообщение
             */
            Message operator = (Message mes){
                this->id = mes.id;
                this->data = mes.data;
                this->size = mes.size;
                this->type = mes.type;
                this->client = mes.client;
                this->partIndex = mes.partIndex;
                this->partsCount = mes.partsCount;
                return mes;
            }

            //! id сообщения (используется для проверки повторных отправок и прочих сбоев)
            MessageIdType id;
            //! \brief Количество частей, на которые разбито большое сообщение
            int partsCount;
            //! \brief Индекс конкретно этой части, передаваемой в этом сообщении
            int partIndex;
            //! тип сообщения
            MessageType type;
             //! данные сообщения
            SymbolType* data;
            //! размер данных сообщения
            int size;
            //! Клиент-отправитель
            Client client;
        };

    }


}

#endif // MESSAGE_H
