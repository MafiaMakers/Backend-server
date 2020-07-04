#ifndef CHATDATABASEMANAGER_H
#define CHATDATABASEMANAGER_H

#include "databasemanager.h"
#include "chatmessage.h"
#include "Network/message.h"
#include "Network/mainservernetworker.h"

namespace Mafia {
    namespace Database {
        //! \brief Максимальное количество сообщений, на которые можно ответить одним сообщением
        const int MAX_ANSWER_MESSAGE_COUNT = 10;
        //! \brief Максимальное количество пользователей в одном чате
        const int MAX_USERS_IN_CHAT = 128;
        //! \brief Основной класс для работы с базой данных сообщений чатов
        class ChatDatabaseManager : public QObject, public DatabaseManager
        {
        Q_OBJECT
        private:
            //! \brief Количество сообщений, которые загружаются стандартно. В вызове функции get_last_messages.
            //! То есть если не указано значение, то будет загружено ровно столько сообщений (или меньше, если их всего меньше)
            static const int MESSAGES_TO_UPLOAD_DEFAULT = 20;
        public:

            /*!
             * \brief Основной конструктор, создающий БД, если ее нет, и подключающийся к ней, если она уже есть
             * \param databaseWorker Указатель на объект для работы с БД
             * \param parent Qt-шная фигня, которая равна nullptr
             */
            ChatDatabaseManager(DatabaseWorker* databaseWorker, QObject* parent = nullptr);

            /*!
             * \brief Функция получения нескольких последних сообщений данного чата
             * \param chatId id чата, из которого необходимо получить сообщения
             * \param messagesCount Количество сообщений, которое необходимо получить.
             * Стандартно равно \ref Mafia::Database::ChatDatabaseManager::MESSAGES_TO_UPLOAD_DEFAULT
             * \return Список из сообщений
             */
            MafiaList<ChatMessage> get_last_messages(ChatIdType chatId, int messagesCount = MESSAGES_TO_UPLOAD_DEFAULT);

            /*!
             * \brief Функция для добавления сообщения в БД сообщений
             * \param message Сообщение, которое надо добавить
             * (у него могут быть не инициализированы поля: id, timestamp и readUsers - они будут инициализированы автоматически)
             * \return id добавленного сообщения
             */
            MessageIdType add_message(ChatMessage message);

            /*!
             * \brief Функция удаления сообщения
             * \param id id сообщения, которое следует удалить
             */
            void delete_message(MessageIdType id);

            /*!
             * \brief Функция для редактирования сообщения.
             * \param message Сообщение, которое следует изменить. Поля readUsers и timestamp нинициализируются автоматически
             */
            void edit_message(ChatMessage message);

            /*!
             * \brief Функция для помечения сообщения как прочитанного кем-либо
             * \param id id сообщения, которое прочитанно
             * \param readUser id пользователя, который его прочитал
             */
            void message_read(MessageIdType id, UserIdType readUser);

            /*!
             * \brief Функция получения сообщений по фильтру
             * \param fromChats Чаты, из которых будут взяты сообщения (если не назначить, то поиск будет по всем чатам)
             * \param possibleSenders Сообщения только этих пользователей будут возвращены
             * (если не назначить, то поиск будет по всем отправителям)
             * \param features Возможные доп. параметры сообщений (если не назначить, то подойдут любые параметры)
             * \param containsData Сообщение должно содержать в себе данную подстроку
             * \param sentAfter Сообщение должно быть отправлено после данного времени
             * \param sentBefore Сообщение должно быть отправлено до данного времени
             * \return Список сообщений, удовлетворяющих условиям (например, get_messages() - это получение всех сообщений)
             */
            MafiaList<ChatMessage> get_messages(MafiaList<ChatIdType> fromChats = MafiaList<ChatIdType>(),
                                                MafiaList<UserIdType> possibleSenders = MafiaList<UserIdType>(),
                                                MafiaList<ChatFeature> features = MafiaList<ChatFeature>(),
                                                QString containsData = "",
                                                QDateTime sentAfter = BEGINNING_TIME,
                                                QDateTime sentBefore = ENDING_TIME);

            /*!
             * \brief Получение всех параметров сообщения по id
             * \param id id сообщения, о котором необходимо получить информацию
             * \return Все данные сообщения
             */
            ChatMessage get_message(MessageIdType id);
        signals:
            /*!
             * \brief Сигнал, вызываемый при успешном добавлении сообщения в БД
             * \param sentMessage Сообщение, которое было добавлено
             */
            void on_message_sent(ChatMessage sentMessage);

            /*!
             * \brief Сигнал, вызываемый при успешном изменении сообщения
             * \param editedMessage Сообщение, которое было изменено
             */
            void on_message_edited(ChatMessage editedMessage);

            /*!
             * \brief Сигнал, вызываемый при удалении сообщения
             * \param deletedId id удаленного сообщения
             * \param chatId id чата, из которого было удалено сообщение
             */
            void on_message_deleted(MessageIdType deletedId, ChatIdType chatId);

            /*!
             * \brief Сигнал, вызываемый при прочтении кем-либо сообщения
             * \param messageRead id сообщения, которое было прочитано
             * \param user пользователь, который прочитал это сообщение
             * \param chat чат, в котором находится это сообщение
             */
            void on_message_read(MessageIdType messageRead, UserIdType user, ChatIdType chat);

        private:
            /*!
             * \brief Метод проверки, совпадают ли эти сообщения
             * (если одно из них является отредактированной копией другого)
             * Проверяется при попытке редактирования сообщения.
             * В случае, если что-либо не совпадает, метод выбрасывает исключение. Если все ОК, то метод ничего не делает
             * \param newMessage Новая версия сообщения
             * \param oldMessage Старая версия сообщения
             */
            void check_message_matches(ChatMessage newMessage, ChatMessage oldMessage);

            /*!
             * \brief Функция генерации запроса, который следует добавить в "WHERE (", чтобы получить сообщения только из этих чатов
             * \param chats Список чатов из которых требуется получить сообщение
             * \return Строку с условием, которое следует добавить в SQL-запрос
             */
            QString generate_request_chat(MafiaList<ChatIdType> chats);

            //! \brief Аналогично с \ref Mafia::Database::ChatDatabaseManager::generate_request_chat но только фильтрует по отправителям
            QString generate_request_sender(MafiaList<UserIdType> senders);

            //! \brief Аналогично с \ref Mafia::Database::ChatDatabaseManager::generate_request_chat но только фильтрует по особенностям сообщений
            QString generate_request_feature(MafiaList<ChatFeature> features);

            /*!
             * \brief Получение сообщений по полученному ответу на sql-запрос
             * \param query sql-запрос, содержащий ответ
             * \return Список из  сообщений
             */
            MafiaList<ChatMessage> get_query_messages(QSqlQuery* query);

        };
    }
}



#endif // CHATDATABASEMANAGER_H
