#ifndef CHATSETTINGSDATABASEMANAGER_H
#define CHATSETTINGSDATABASEMANAGER_H

#include "databasemanager.h"
#include "user.h"

namespace Mafia {
    namespace Database {
        //! \brief Перечисление возможных должностей пользователя в чате
        enum ChatCapability{
            //! \brief Индекс первого элемента перечисления (см. )
            ChatCapabilities_First = 0,
            //! \brief Просто наблюдатель (не может отправлять сообщения)
            ChatCapabilities_Watcher = 0,
            //! \brief Обычный пользователь (может читать и отправлять сообщения)
            ChatCapabilities_Speaker = 1,
            //! \brief Может редактировать свои и (?чужие?) сообщения
            ChatCapabilities_Editor = 2,
            //! \brief Админ (может добавлять, удалять, назначать должности разных пользователей)
            ChatCapabilities_Admin = 3,
            //! \brief Индекс последнего элемента перечисления (см. )
            ChatCapabilities_Last = 4
        };
        //! \brief Структура для хранения данных чата
        struct Chat{
            //! \brief id чата
            ChatIdType id;
            //! \brief Список id всех пользователей, состоящих в чате
            MafiaList<UserIdType> users;
            //! \brief Список должностей пользователей (usersCapabilities[i] - это должность i-го пользователя)
            MafiaList<ChatCapability> usersCapabilities;
            //! \brief Дата и время создания чата
            QDateTime creationTime;
            //! \brief Функция для вывода в консоль всех полей чата
            void show();
        };

        //! \brief Основной класс для работы с БД чатов
        class ChatSettingsDatabaseManager : public DatabaseManager
        {
        public:
            /*!
             * \brief Основной конструктор, который создает, если надо, и подключается к БД
             * \param databaseWorker указатель на объект для работы с БД
             */
            ChatSettingsDatabaseManager(DatabaseWorker* databaseWorker);

            /*!
             * \brief Функция для создания нового пустого чата
             * \return id созданного чата
             */
            ChatIdType create_chat();

            /*!
             * \brief Функция добавления пользователя в чат
             * \param user id пользователя, которого надо добавить в чат
             * \param chat id чата, в который надо добавить пользователя
             * \param usersCapability должность пользователя, которую он получит в этом чате
             * (стандартно - \ref Mafia::Database::ChatCapabilities_Speaker)
             */
            void add_user_to_chat(UserIdType user, ChatIdType chat, ChatCapability usersCapability = ChatCapabilities_Speaker);

            /*!
             * \brief Функция назначения новой должности пользователю
             * \param user Пользователь, которому следует назначить эту должность
             * \param chat Чат, в котором надо назначить должность
             * \param newCapability Новая должность
             */
            void set_capability(UserIdType user, ChatIdType chat, ChatCapability newCapability);

            /*!
             * \brief Удаление пользователя из чата
             * \param user id пользователя, которого следует удалить
             * \param chat id чата, из которого стоит удалить пользователя
             */
            void remove_user_from_chat(UserIdType user, ChatIdType chat);

            /*!
             * \brief Функция получения всех чатов БД
             * \return Список из всех чатов
             */
            MafiaList<Chat> get_all_chats();

            /*!
             * \brief Получение всех чатов, в которых состоит данный пользователь
             * \param user id пользователя
             * \return Список всех чатов
             */
            MafiaList<Chat> get_users_chat(UserIdType user);

            /*!
             * \brief Получение чата по id
             * \param chat id чата
             * \return Все данные о чате
             */
            Chat get_chat(ChatIdType chat);

        private:
            /*!
             * \brief Получение всех чатов из sql-запроса
             * \param query указатель на sql-запрос с ответом
             * \return Список всех чатов, которые были в sql-запросе
             */
            MafiaList<Chat> get_request_chat(QSqlQuery* query);

        };
    }
}


#endif // CHATSETTINGSDATABASEMANAGER_H
