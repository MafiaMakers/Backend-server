#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>
#include "Gameplay/gameitems.h"

namespace Mafia {
    namespace Database {

        //! \brief Тип, использующийся для id пользователей в базе данных
        typedef int UserIdType;
        //! \brief Тип, использующийся для id чатов в базе данных
        typedef int ChatIdType;
        //! \brief Тип, использующися для id транзакций в базе данных
        typedef int TransactionIdType;
        //! \brief Тип, использующийся для хранения id игр в базе данных
        typedef int GameIdType;
        //! \brief Тип, использующийся для хранения ключа подтверждения email-а в базе данных
        typedef QString ConfirmationKeyType;

        //! \brief Возможные статусы игрока
        enum Status{
            //! \brief Индекс первого статуса. Может использоваться для цикла for:
            //! \brief for(Status status = Status_First; status < Status_Last; status++)
            Status_First = 0,
            //! \brief Статус, который появляется у пользователя при регистрации, статус неподтвержденного email-а
            Status_NotConfirmed = 0,
            //! \brief Статус, говорящий о том, что email подтвержден
            Status_Confirmed = 1,
            //! \brief Индекс последнего статуса. Может использоваться в цикле for (см. \ref Mafia::Database::Status_First)
            Status_Last = 2
        };

        //! \brief Возможные типы аккаунтов
        enum AccountType{
            //! \brief Индекс первого типа аккаунта (см. \ref Mafia::Database::Status_First)
            AccountType_First = 0,
            //! \brief Обычный аккаунт
            AccountType_Free = 0,
            //! \brief Премиум аккаунт
            AccountType_Pro = 1,
            //! \brief Индекс последнего типа аккаунта (см. \ref Mafia::Database::Status_First)
            AccountType_Last = 2

        };

        //! \brief Возможные достижения (ну или что-то вроде нашивок)
        enum Achievement{
            //! \brief Индекс первого достижения (см. \ref Mafia::Database::Status_First)
            Achievement_First = 0,
            //! \brief Никакого достижения
            Achievement_None = 0,
            //! \brief Просто рандомное достижения, пока заглушка
            Achievement_ProMLGPlayer = 1,
            //! \brief Индекс последнего достижения (см. \ref Mafia::Database::Status_First)
            Achievement_Last = 2
        };

        //! \brief Структура, используемая для хранения и работы с транзакциями
        struct Transaction{
            //! \brief id транзакции
            TransactionIdType id;
            //! \brief id пользователя, который является плательщиком
            UserIdType buyer;
            //! \brief цена, которая была уплачена
            int price;
            //! \brief Хеш транзакции, чтобы ее было сложнее подменить или вставить среди других транзакций
            QString hash;
        };

        //! \brief Структура, используемая для хранения и работы с пользователем
        struct User{
            //! \brief id пользователя (уникальный)
            UserIdType id;
            //! \brief имя пользователя (не уникальное!)
            QString nickname;
            //! \brief email пользователя (уникальный)
            QString email;
            //! \brief Хеш пароля для сверения его с вводимым паролем
            QString passwordHash;
            //! \brief Соль хеша пароля (для большей безопасности)
            QString salt;
            //! \brief Статус пользователя (подтвердил ли он свой email)
            Status isConfirmed;
            //! \brief Авторизован ли прям сейчас пользователь в игре
            bool authorized;
            //! \brief Тип аккаунта пользователя
            AccountType accountType;
            //! \brief Дата и время регистрации пользователя
            QDateTime loginDateTime;
            //! \brief Достижение (нашивка) пользователя
            Achievement achievement;
            //! \brief Ключ для подтверждения email-а
            ConfirmationKeyType confirmationKey;
            //! \brief Список всех транзакций, проведенных пользователем
            QList<TransactionIdType> transactions;
            //! \brief Список всех id игр, в которых участвовал пользователь
            QList<GameIdType> allGames;
            //! \brief Количество побед в играх (по каждой роли. То есть victoriesByRoles[RoleId] - это количество побед ролью RoleId)
            QList<int> victoriesByRoles;
            //! \brief Количество поражений в играх (по каждой роли)
            QList<int> defeatesByRoles;
            //! \brief Список всех id чатов, в которых участвует пользователь
            QList<ChatIdType> chats;
        };
    }
}

#endif // USER_H
