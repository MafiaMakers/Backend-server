#ifndef USERSTATISTICS_H
#define USERSTATISTICS_H
#include "System/functions.h"
#include <MafiaDatabase>
namespace Mafia {
    /*!
     * \brief Структура, используемая для отправки пользователям статистики об игроке
     */
    struct UserStatistics{
        /*!
         * \brief Пустой конструктор
         */
        UserStatistics();
        /*!
         * \brief Конструктор, преобразующий все данные о пользователе в статистику, которую можно отправить всем
         * (исключает конфиденциальные и системные данные)
         * \param user Данные о пользователе, которые следует преобразовать
         */
        UserStatistics(Database::User user);
        //! \brief см. \ref Mafia::Database::User::victoriesByRoles
        MafiaList<int> victoriesByRoles;
        //! \brief см. \ref Mafia::Database::User::defeatesByRoles
        MafiaList<int> defeatesByRoles;
        //! \brief см. \ref Mafia::Database::User::loginDateTime
        QDateTime loginDateTime;
        //! \brief см. \ref Mafia::Database::User::accountType
        Database::AccountType accountType;
        //! \brief см. \ref Mafia::Database::User::achievement
        Database::Achievement achievement;
        //! \brief см. \ref Mafia::Database::User::id
        Database::UserIdType id;
        //! \brief см. \ref Mafia::Database::User::nickname
        QString nickname;
    };
}

#endif // USERSTATISTICS_H
