#ifndef GAMEDATABASEWORKER_H
#define GAMEDATABASEWORKER_H

#include "databasemanager.h"
#include "Gameplay/game_s.h"

namespace Mafia {
    namespace Database {

        //! \brief Основной класс для работы с БД с играми
        class GameDatabaseManager : public DatabaseManager
        {
        public:
            /*!
             * \brief Основной конструктор, который создает, если надо, и связывается с таблицей в БД
             * \param databaseWorker Указатель на объект для работы с БД
             */
            GameDatabaseManager(DatabaseWorker* databaseWorker);

            /*!
             * \brief Получение всей информации об одной игре
             * \param id id игры
             * \return Все данные об игре
             */
            Gameplay::Game get_game_data(GameIdType id);

            /*!
             * \brief Функция для добавления новой игры в БД
             * \param game Все данные об этой новой игре (id может быть неинициализирован, его автоматически инициализирует функция)
             * \return id добавленной игры
             */
            GameIdType add_game(Gameplay::Game &game);

            /*!
             * \brief Функция для получения игр по фильтру. Ищет все игры, удовлетворяющие условию
             * \param participants Участники, участие которых необходимо для выбора игры (если не назначать, то будут подходить любые)
             * \param participantsFilter Тип фильтра для участников (необходимо участие всех участников из списка в одной конкретной игре
             * или хотя бы одного из них, чтобы взять игру в список)
             * \param roles Роли, которые должны были быть розданы кому-либо (хоть одному) для выбора игры
             * (если не назначать, то будут подходить любые)
             * \param rolesFilter Тип фильтра для ролей (необходимо наличие всех ролей из списка или хотя бы одной)
             * \param outcomes Возможные исходы (чем игра закончилась) (если не назначать, то подойдут любые)
             * \param beginAfter Подойдут только игры, начавшиеся после данного времени
             * \param endBefore Подойдут только игры, закончившиеся до данного времени
             * \return Список из всех игр, подходящих под условия (например, get_games_with() - это получение всех игр)
             */
            MafiaList<Gameplay::Game> get_games_with(MafiaList<UserIdType> participants = MafiaList<UserIdType>(),
                                                 FilterType participantsFilter = FilterType_NONE,
                                                 MafiaList<Gameplay::Role> roles = MafiaList<Gameplay::Role>(),
                                                 FilterType rolesFilter = FilterType_NONE,
                                                 MafiaList<Gameplay::GameResult> outcomes = MafiaList<Gameplay::GameResult>(),
                                                 QDateTime beginAfter = BEGINNING_TIME,
                                                 QDateTime endBefore = ENDING_TIME);

        private:
            /*!
             * \brief Функция создания строки с запросом для добавления в "WHERE(". Для фильтрации только игр с данными пользователями.
             * \param participants Список пользователей
             * \param filter Тип фильтра пользователей (либо все перечисленные пользователи должны быть в каждой игре (AND),
             * либо хоть кто-то один из них должен быть в каждой игре (OR))
             * \return Строку, в которой записан запрос
             */
            QString generate_request_participant(MafiaList<UserIdType> participants, FilterType filter);

            /*!
             * \brief Функция создания строки с запросом для добавления в "WHERE(". Для фильтрации только игр с данными исходами.
             * \param outcomes Список подходящих исходов
             * \return Строку, в которой записан запрос
             */
            QString generate_request_outcomes(MafiaList<Gameplay::GameResult> outcomes);

            /*!
             * \brief Функция создания строки с запросом для добавления в "WHERE(". Для фильтрации только игр с данными ролями.
             * \param roles Список ролей
             * \param filter Тип фильтра ролей (либо все перечисленные роли должны быть в каждой игре (AND),
             * либо хоть какая-то роль из списка должна быть в каждой игре (OR))
             * \return Строку, в которой записан запрос
             */
            QString generate_request_roles(MafiaList<Gameplay::Role> roles, FilterType filter);
        };
    }
}


#endif // GAMEDATABASEWORKER_H
