#ifndef USERDATABASEWORKER_H
#define USERDATABASEWORKER_H

#include "databaseworker.h"
#include "user.h"
#include "Gameplay/game_s.h"
#include "databasehelper.h"
#include "databasemanager.h"

//! \brief Максимальное количество транзакций одного игрока
#define MAX_TRANSACTIONS_COUNT 1024
//! \brief Максимальное количество игр одного игрока
#define MAX_GAMES_COUNT 65536
//! \brief Максимальное количество чатов, в которых может состоять игрок
#define MAX_CHATS_COUNT 1024

namespace Mafia{
    namespace Database {
         /*!
         * \brief Класс для работы с базой данных пользователя
         */
        class UserDatabaseManager : public DatabaseManager
        {
        public:
            /*!
             * \brief Основной конструктор, который принимает и инициализирует объект DatabaseWorker для работы с БД
             * \param _dbWorker указатель на объект для работы с БД
             */
            UserDatabaseManager(DatabaseWorker* _dbWorker);

            /*!
             * \brief Функция добавления нового пользователя в БД
             * \param nickname имя пользователя
             * \param email email пользователя
             * \param password пароль пользователя
             * \return id нового пользователя в базе данных
             */
            UserIdType add_user(QString nickname, QString email, QString password);

            /*!
             * \brief Функция для входа пользователя в учетную запись
             * \param id id пользователя
             * \param password пароль пользователя
             * \return true, если пользователь успешно вошел, false, если произошла ошибка или пароль не подходит
             */
            bool login_user(UserIdType id, QString password);

            /*!
             * \brief Функция для выхода пользователя из игры
             * \param id id пользователя
             */
            void logout_user(UserIdType id);

            /*!
             * \brief Подтверждение адреса электронной почты пользователем
             * \param id id пользователя
             * \param confirmationKey Ключ подтверждения, который ввел пользователь
             * \return true, если пользователь подтвердил свой email и false, если произошла ошибка или ключ неверный
             */
            bool confirm_user(UserIdType id, QString confirmationKey);

            /*!
             * \brief Собирает все данные о пользователе по id
             * \param id id пользователя
             * \return Структуру, содержащую все данные о пользователе
             */
            User get_user(UserIdType id);

            /*!
             * \brief Добавление в БД новой игры (касается только пользователей! В БД с играми ничего добавлено не будет!!!)
             * \param game Все данные об игре
             */
            void register_game(Gameplay::Game game);

            /*!
             * \brief Добавление транзакции в БД (касается только пользователей! В БД с транзакциями ничего добавлено не будет!!!)
             * \param transaction
             */
            void add_transaction(UserIdType user, TransactionIdType transaction);

            /*!
             * \brief Изменение пароля пользователя
             * \param id id пользователя, у которого следует изменить пароль
             * \param newPassword Новый пароль, который следует установить
             */
            void change_password(UserIdType id, QString newPassword);

            /*!
             * \brief Получение id пользователя по email-у
             * \param email email пользователя
             * \return id пользователя с указанным email-ом
             */
            UserIdType get_id(QString email);

            /*!
             * \brief Изменение достижения (нашивки) пользователя
             * \param id id пользователя
             * \param achievement Достижение (нашивка), которое следует назначить
             */
            void add_achievement(UserIdType id, Achievement achievement);

            /*!
             * \brief Добавление пользователя в указанный чат
             * \param userId id пользователя
             * \param chatId id чата
             */
            void add_user_to_chat(UserIdType userId, ChatIdType chatId);

            /*!
             * \brief Удаление пользователя из чата
             * \param userId id пользователя
             * \param chatId id чата
             */
            void remove_user_from_chat(UserIdType userId, ChatIdType chatId);

            /*!
             * \brief Назначение типа аккаунта пользователя
             * \param id id пользователя
             * \param newAccountType Тип аккаунта, который следует назначить
             */
            void set_account_type(UserIdType id, AccountType newAccountType);

            /*!
             * \brief Функция изменения адреса электронной почты пользователя
             * \param id id пользователя
             * \param newEmail новый электронный адрес пользователя
             * \return true, если изменение прошло успешно и false, если такой адрес уже занят или произошла ошибка
             */
            bool change_email(UserIdType id, QString newEmail);

            /*!
             * \brief Функция изменения имени пользователя
             * \param id id пользователя
             * \param newNickname новое имя пользователя
             */
            void change_nickname(UserIdType id, QString newNickname);

            /*!
             * \brief Получение списка пользователей, удовлетворяющих фильтрам
             * \param ids Все id, среди которых будет идти поиск подходящих пользователей
             * (если не назначить, то поиск будет по всем пользователям)
             * \param userStatus Статус, которым должны обладать все пользователи (если не назначить, то подойдет любой статус)
             * \param userAchievement Достижение (нашивка), которой должны обладать все пользователи
             * (если не назначить, то подойдет любое достижение (нашивка))
             * \param authorizedNow Авторизован ли сейчас пользователь (если не назначить, то подойдут и те и другие)
             * \param nickname Никнейм пользователя должен содержать эту подстроку
             * (если не назначить или оставить "", то подойдет любой никнейм)
             * \param loginAfter Пользователь должен быть зарегистрирован позднее, чем в эту дату
             * (если не назначить, то границы снизу не будет)
             * \param loginBefore Пользователь должен быть зарегистрирован до этой даты (если не назначить, то границы сверху не будет)
             * \return Список всех пользователей, которые подходят условиям
             */
            MafiaList<User> get_users(MafiaList<UserIdType> ids = MafiaList<UserIdType>(),
                                      Status userStatus = Status_Any,
                                      Achievement userAchievement = Achievement_Any,
                                      AuthorizedStatus authorizedNow = AuthorizedStatus_Any,
                                      QString nickname = "",
                                      QDateTime loginAfter = BEGINNING_TIME,
                                      QDateTime loginBefore = ENDING_TIME);

        private:
            /*!
             * \brief Получение списка пользователей по ответу на sql-запрос
             * \param query Sql-запрос с ответом из которого надо извлечь данные
             * \return Список пользователей, которые были полученны запросом
             */
            MafiaList<User> get_query_users(QSqlQuery* query);

            /*!
             * \brief Добавление пользователя в БД
             * \param user все данные о пользователе
             * \return id пользователя, добавленного в БД
             */
            UserIdType add_user(User user);

            /*!
             * \brief Функция рандомной генерации ключа подтверждения email-a
             * \return Строку с ключем
             */
            static QString generate_confirmation_key();

            /*!
             * \brief Функция добавления информации о новой игре конкретному пользователю (участнику этой игры)
             * \param userId id пользователя
             * \param gameId id игры, которую следует добавить
             * \param role Роль, которая была у пользователя в этой игре
             * \param result Персональный результат этого пользователя в этой игре
             */
            void register_game(UserIdType userId, GameIdType gameId, Gameplay::Role role, Gameplay::GamePersonalResult result);

        };
    }
}



#endif // USERDATABASEWORKER_H
