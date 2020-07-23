#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>
#include "Gameplay/gameitems.h"

namespace Mafia {
	namespace Database {
		//! \brief Формат даты и времени, который используется везде по программе (т.к. он используется SQL)
		const QString SQL_DATETIME_FORMAT = "yyyy.MM.dd hh:mm:ss";
        //! \brief Время, которое мы считаем 'самым началом' (раньше него не может быть ничего)
		const QDateTime BEGINNING_TIME = QDateTime::fromString("2000.01.01 01:01:01", SQL_DATETIME_FORMAT);
        //! \brief Время, которое мы считаем 'самым концом' (позже него не может быть ничего)
        const QDateTime ENDING_TIME = QDateTime::fromString("2073.01.01 01:01:01", SQL_DATETIME_FORMAT);


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
        //! \brief Тип, используемый для хранения цены, заплаченной при транзакции
        typedef int PriceType;

        //! \brief Возможные статусы игрока
        enum Status{
            //! \brief Индекс первого статуса. Может использоваться для цикла for:
            //! \brief for(Status status = Status_First; status < Status_Last; status++)
            Status_First = 0,
            //! \brief Любой возможный статус (используется в фильтрах)
            Status_Any = 0,
            //! \brief Статус, который появляется у пользователя при регистрации, статус неподтвержденного email-а
            Status_NotConfirmed = 1,
            //! \brief Статус, говорящий о том, что email подтвержден
            Status_Confirmed = 2,
            //! \brief Индекс последнего статуса. Может использоваться в цикле for (см. \ref Mafia::Database::Status_First)
            Status_Last = 3
        };

        //! \brief Возможные типы аккаунтов
        enum AccountType{
            //! \brief Индекс первого типа аккаунта (см. \ref Mafia::Database::Status_First)
            AccountType_First = 0,
            //! \brief Любой возможный тип аккаунта (используется в фильтрах)
            AccountType_Any = 0,
            //! \brief Обычный аккаунт
            AccountType_Free = 1,
            //! \brief Премиум аккаунт
            AccountType_Pro = 2,
            //! \brief Индекс последнего типа аккаунта (см. \ref Mafia::Database::Status_First)
            AccountType_Last = 3
        };

        //! \brief Возможные достижения (ну или что-то вроде нашивок)
        enum Achievement{
            //! \brief Индекс первого достижения (см. \ref Mafia::Database::Status_First)
            Achievement_First = 0,
            //! \brief Любой возможный тип достижения (нашивки) (используется в фильтрах)
            Achievement_Any = 0,
            //! \brief Никакого достижения
            Achievement_None = 1,
            //! \brief Просто рандомное достижения, пока заглушка
            Achievement_ProMLGPlayer = 2,
            //! \brief Индекс последнего достижения (см. \ref Mafia::Database::Status_First)
            Achievement_Last = 3
        };

        //! \brief Тип транзакции (на случай разных транзакций - оплат, возвратов и т.п.)
        enum TransactionType{
            //! \brief Индекс первого элемента перечисления (см. )
            TransactionType_First = 0,
            //! \brief Обычная покупка
            TransactionType_BuySomething = 0,
            //! \brief Индекс последнего элемента перечисления (см. )
            TransactionType_Last = 1
        };

        //! \brief Статус авторизованности пользователя (находится ли прямо сейчас пользователь в игре)
        enum AuthorizedStatus{
            //! \brief Индекс первого элемента перечисления (см. )
            AuthorizedStatus_First = 0,
            //! \brief Любой статус авторизованности (используется в фильтрах)
            AuthorizedStatus_Any = 0,
            //! \brief Пользователь не авторизован
            AuthorizedStatus_NotAuthorized = 1,
            //! \brief Пользователь авторизован
            AuthorizedStatus_Authorized = 2,
            //! \brief Индекс последнего элемента перечисления (см. )
            AuthorizedStatus_Last = 3
        };

        //! \brief Структура, используемая для хранения и работы с транзакциями
        struct Transaction{
            //! \brief id транзакции
            TransactionIdType id;
            //! \brief id пользователя, который является плательщиком
            UserIdType buyer;
            //! \brief цена, которая была уплачена
            PriceType price;
            //! \brief Хеш транзакции, чтобы ее было сложнее подменить или вставить среди других транзакций
            QString hash;
            //! \brief Время и дата совершения транзакции
            QDateTime timestamp;
            //! \brief Тип транзакции (см. \ref Mafia::Database::TransactionType)
            TransactionType type;
            //! \brief Метод для вывода в консоль всех данных о транзакции
            void show();

			bool operator == (const Transaction &a) const;
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
            AuthorizedStatus authorized;
            //! \brief Тип аккаунта пользователя
            AccountType accountType;
            //! \brief Дата и время регистрации пользователя
            QDateTime loginDateTime;
            //! \brief Достижение (нашивка) пользователя
            Achievement achievement;
            //! \brief Ключ для подтверждения email-а
            ConfirmationKeyType confirmationKey;
            //! \brief Список всех транзакций, проведенных пользователем
            MafiaList<TransactionIdType> transactions;
            //! \brief Список всех id игр, в которых участвовал пользователь
            MafiaList<GameIdType> allGames;
            //! \brief Количество побед в играх (по каждой роли. То есть victoriesByRoles[RoleId] - это количество побед ролью RoleId)
            MafiaList<int> victoriesByRoles;
            //! \brief Количество поражений в играх (по каждой роли)
            MafiaList<int> defeatesByRoles;
            //! \brief Список всех id чатов, в которых участвует пользователь
            MafiaList<ChatIdType> chats;
            //! \brief Метод для вывода в консоль всех данных о пользователе
            void show();

			bool operator == (const User &user) const;

			bool operator != (const User &user) const;
        };
    }
}

#endif // USER_H
