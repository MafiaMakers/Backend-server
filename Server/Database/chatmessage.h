#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include "user.h"

namespace Mafia {
    namespace Database {
        //! \brief Перечисление возможных параметров сообщения
        enum ChatFeature{
            //! \brief Никакого параметра
            ChatFeature_None,
            //! \brief Измененное сообщение
            ChatFeature_Edited,
            //! \brief Помеченное сообщение
            ChatFeature_Marked
        };

        //! \brief Тип, используемый для обозначения id сообщений чата
        typedef int MessageIdType;

        //! \brief Структура для хранения в БД и работы с сообщениями чата
        struct ChatMessage{
            //! \brief id сообщения
            MessageIdType id;
            //! \brief Само сообщение (текст)
            QString data;
            //! \brief id пользователя-отправителя
            UserIdType from;
            //! \brief id чата, в который отправлено сообщение
            ChatIdType toChat;
            //! \brief Время, когда было отправлено сообщение
            QDateTime timestamp;
            //! \brief Дополнительный параметр сообщения (см. \ref Mafia::Database::ChatFeature)
            ChatFeature feature;
            //! \brief Список сообщений, на которые отвечает данное сообщение
            MafiaList<MessageIdType> answerFor;
            //! \brief Список пользователей, которые прочитали это сообщение
            MafiaList<UserIdType> readUsers;
            //! \brief Метод для вывода информации о сообщении (всех его полей)
            void show();

			bool operator == (const ChatMessage &a) const;
        };
    }
}


#endif // CHATMESSAGE_H
