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
        //! \brief ip отправителя сообщения
        int ip;
        //! \brief порт, с которого было отправлено сообщение
        int port;
    };

    /*!
     * \brief Основная структура для сообщений
     */
    struct Message{
        //! id сообщения (используется для проверки повторных отправок и прочих сбоев)
        MessageIdType id;
        //! тип сообщения
        MessageTypeType type;
         //! данные сообщения
        SymbolType* data;
        //! размер данных сообщения
        int size;
        //! Клиент-отправитель
        Client from;
    };

}

#endif // MESSAGE_H
