#ifndef MAINSERVERMANAGER_H
#define MAINSERVERMANAGER_H
#include <MafiaGameplay>
#include <MafiaNetwork>
#include <MafiaRequests>
#include <MafiaSubservers>
#include <MafiaSystem>
#include <MafiaExceptions>
#include <MafiaDatabase>
#include <thread>
#include <chrono>

namespace Mafia {
    /*!
     * \brief Главный управляющий класс основного сервера. Он пока еще не дописан. Только начало...
     */
    class MainServerManager : public QObject
    {
        Q_OBJECT
    public:
        /*!
         * \brief Конструктор, который также создает \ref MainServerNetworker и пока что для теста еще создает 1 тестовый запрос
         */
        explicit MainServerManager(int argc, char* argv[], QObject * parent = nullptr);

    private slots:
        void on_message_received(Network::Message message);

    private:
        void create_user(QString nickname, QString email, QString password, Client client);

        void send_chat_message(Database::UserIdType sender, Database::ChatIdType toChat, QString data, QList<Database::MessageIdType> answerFor,
                               Database::ChatFeature feature);

        void login_user(QString email, QString password, Client client);

        void create_chat(Database::UserIdType creator);

        void get_last_messages(Database::UserIdType user, Database::ChatIdType chat, int messagesCount = -1);

        void get_users_chats(Database::UserIdType user, int chatsCount = CHATS_UPLOAD_COUNT);

        void add_user_to_chat(Database::ChatIdType chat, Database::UserIdType user, Database::UserIdType initializer,
                              Database::ChatCapability capability = Database::ChatCapabilities_Speaker);

        void remove_user_from_chat(Database::ChatIdType chat, Database::UserIdType user, Database::UserIdType initializer);

        void change_users_chat_capability(Database::ChatIdType chat, Database::UserIdType user,
                                          Database::ChatCapability newCapability, Database::UserIdType initializer);

        void create_game(Client creator);

        void get_statistics(Database::UserIdType user, Database::UserIdType asker);

        void add_game(Gameplay::Game game);

        void logout_user(Database::UserIdType user);

        void get_logs_data(QString data, Client sender);

        void add_transaction(Database::Transaction transaction);

        void change_nickname(Database::UserIdType user, QString newNickname);

        void change_email(Database::UserIdType user, QString newEmail);

        void change_achievement(Database::UserIdType user, Database::Achievement achievement);

        void add_user_to_game(Client client, int gameId);

        void delete_message(Database::UserIdType user, Database::MessageIdType message);

        void edit_message(Database::UserIdType user, Database::ChatMessage message);

        void read_message(Database::UserIdType user, Database::MessageIdType message);
        /*!
         * \brief Получает данные от уже инициализированного сетевого запроса
         * \param req Указатель на запрос
         */
        void _get_data_from_request(Requests::NetworkRequest* req);

        //! \brief Тестовая функция для теста БД (создает несколько тестовых элементов, добавляет их в БД и выводит обратно)
        void _database_test();

        //! \brief Функция, получающая все данные из БД и выводящая их в консоль
        void _database_get_all();

        //! \brief Тестовая функция для проверки субсервера, запросов и нетворкера
        //! (создает их объекты, отправляет тестовый запрос, запускает пробный субсервер и отслеживает его работу)
        void _networker_test();

        Client get_client_by_user(Database::UserIdType user);

        Database::UserIdType get_user_by_client(Client client);

        MafiaList<Client> clients;

        MafiaList<Database::UserIdType> users;

        //! \brief Указатель на объект сетевого класса
        MainServerNetworker* networker;

        //! \brief Объект для работы с общей БД
        Database::DatabaseWorker* dbWorker;
        //! \brief Объект для работы с БД пользователей
        Database::UserDatabaseManager* usersDb;
        //! \brief Объект для работы с БД сообщений чатов
        Database::ChatDatabaseManager* chatsDb;
        //! \brief Объект для работы с БД игр
        Database::GameDatabaseManager* gamesDb;
        //! \brief Объект для работы с БД транзакций
        Database::TransactionDatabaseManager* transactionsDb;
        //! \brief Объект для работы с БД чатов
        Database::ChatSettingsDatabaseManager* chatSettingsDb;

        static const Database::UserIdType nullUser = -1;
        static const int CHATS_UPLOAD_COUNT = 20;

    };
}

#endif // MAINSERVERMANAGER_H
