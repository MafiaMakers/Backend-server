#ifndef USERDATABASEWORKER_H
#define USERDATABASEWORKER_H

#include "databaseworker.h"
#include "user.h"
#include "Gameplay/game_s.h"

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
        class UserDatabaseWorker
        {
        public:
            /*!
             * \brief Основной конструктор, который принимает и инициализирует объект DatabaseWorker для работы с БД
             * \param _dbWorker указатель на объект для работы с БД
             */
            UserDatabaseWorker(DatabaseWorker* _dbWorker);

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
            void add_transaction(Transaction transaction);

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

        private:
            /*!
             * \brief Добавление пользователя в БД
             * \param user все данные о пользователе
             * \return id пользователя, добавленного в БД
             */
            UserIdType add_user(User user);

            /*!
             * \brief Функция случайной генерации соли для хеширования пароля
             * \return Соль в виде строки
             */
            static QString generate_salt();

            /*!
             * \brief Функция преобразования QByteArray, полученного из БД в QList<T>
             * \param data данные, полученные из БД в виде QByteArray
             * \return список, сгенерированный на основе этих данных
             */
            template<class T>
            QList<T> qlist_from_byte_array(QByteArray data);

            /*!
             * \brief Функция преобразования QList<T>, полученного из программы в QByteArray для записи в БД
             * \param data Список, который следует преобразовать
             * \return QByteArray, готовый для записи в БД
             */
            template<class T>
            QByteArray qbytearray_from_qlist(QList<T> data);

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

            //! Указатель на объект для работы с базой данных
            DatabaseWorker* dbWorker;
            //! Локальный параметр хеширования
            const QString localParameter = "Par@!A#$^%&*()-_=+/\\MeTerLOCal";
            //! Имя таблицы с пользователями
            static const QString dbName;

        };
    }
}



#endif // USERDATABASEWORKER_H
