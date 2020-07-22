#ifndef GAME_S_H
#define GAME_S_H
#include "Database/user.h"
#include "Database/databasehelper.h"

namespace Mafia {
    namespace Gameplay {
        //! \brief Максимальное количество игроков в одной игре
        const int MAXIMUM_USERS_IN_GAME = 100;

        //! \brief Структура для хранения информации об игре
        struct Game{
            //! \brief id игры
            Database::GameIdType id;
            //! \brief Глобальный результат игры
            GameResult result;
            //! \brief Список всех пользователей, учавствовавших и наблюдавших за игрой
            MafiaList<Database::UserIdType> users;
            //! \brief Список ролей пользователей
            MafiaList<Role> roles;
            //! \brief Дата и время начала игры
            QDateTime beginningDT;
            //! \brief Дата и время окончания игры
            QDateTime endingDT;
            //! \brief Метод вывода в консоль всех параметров игры
            void show();

			bool operator == (const Game &a) const;
        };

        /*!
         * \brief Функция преобразования глобального результата в персональный результат конкретной роли
         * \param role Роль, для которой необходимо узнать персональный результат
         * \param res Глобальный результат игры
         * \return Персональный результат этой роли в исходе - глобальном результате
         */
        GamePersonalResult result_by_role_and_res(Role role, GameResult res);
    }
}


#endif // GAME_S_H
