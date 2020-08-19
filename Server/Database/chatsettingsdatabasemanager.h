#ifndef CHATSETTINGSDATABASEMANAGER_H
#define CHATSETTINGSDATABASEMANAGER_H

#include "databasemanager.h"
//#include "gamedatabasemanager.h"
#include "chatsettings.h"
namespace Mafia {
    namespace Database {

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
             * \brief Получение id всех чатов, подходящих под фильтры
             * \param ids Список возможных id (среди которых искать) (если оставить пустым или задать пустой список, то будет поиск по всем чатам)
             * \param users Пользователи, которые входят в чат
             * \param usersFilter Тип фильтра по пользователям (должны ли в один чат входить сразу все пользователи из списка или достаточно
             * наличия одного пользователя из списка в чате, чтобы чат подходил)
             * \param createdAfter Поиск будет только среди чатов, созданных позднее данного момента
             * \param createdBefore Поиск будет только среди чатов, созданных ранее данного момента
             * \return Список id всех чатов, которые подходят под условие
             */
            MafiaList<ChatIdType> get_chats_with(MafiaList<ChatIdType> ids = MafiaList<ChatIdType>(),
                                                 MafiaList<UserIdType> users = MafiaList<UserIdType>(),
                                                 FilterType usersFilter = FilterType_NONE,
                                                 QDateTime createdAfter = BEGINNING_TIME,
                                                 QDateTime createdBefore = ENDING_TIME);

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

            /*!
             * \brief Функция проверяет, может ли данный пользователь отправлять сообщения в данный чат
             * \param user id пользователя, которого проверяем
             * \param chat id чата, в который пользователь может или не может отправить сообщение
             * \return true, если пользователь может отправлять сообщение в этот чат, иначе false
             */
            bool can_send_message(UserIdType user, ChatIdType chat);

            /*!
             * \brief Функция проверяет, может ли пользователь читать сообщения из данного чата
             * \param user id пользователя, которого проверяем
             * \param chat id чата, который проверяем
             * \return true, если пользователь может читать, иначе false
             */
            bool can_read_message(UserIdType user, ChatIdType chat);

            /*!
             * \brief Функция, проверяющая, может ли пользователь изменять параметры других пользователей в этом чате (назначать админов,
             * добавлять и удалять пользователей из чата)
             * \param user id пользователя, которого проверяем
             * \param chat id чата, который проверяем
             * \return true, если пользователь может изменять права других пользователей, иначе - false
             */
            bool can_edit_users(UserIdType user, ChatIdType chat);

            /*!
             * \brief Функция, проверяющая, может ли данный пользователь изменять чужие сообщения в данном чате
             * \param user id пользователя, которого проверяем
             * \param chat id чата, который проверяем
             * \return true, если пользователь может изменять чужие сообщения в этом чате, иначе - false
             */
            bool can_edit_messages(UserIdType user, ChatIdType chat);

            /*!
             * \brief Получение роли (прав) пользователя в данном чате
             * \param user id пользователя
             * \param chat id чата
             * \return Права или роль пользователя в данном чате (см. \ref Mafia::Database::ChatCapability)
             */
            ChatCapability get_capability(UserIdType user, ChatIdType chat);

        private:
            QString generate_request_id(MafiaList<ChatIdType> ids);

            QString generate_request_user(MafiaList<UserIdType> users, FilterType filter);

            /*!
             * \brief Получение всех чатов из sql-запроса
             * \param query указатель на sql-запрос с ответом
             * \return Список всех чатов, которые были в sql-запросе
             */
			MafiaList<Chat> get_request_chat(QSqlQuery query);
        };
    }
}


#endif // CHATSETTINGSDATABASEMANAGER_H
