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
        /*!
         * \brief Слот, вызываемый при получении от кого-либо сообщения по сети
         * \param message Все сообщение, полученное
         */
        void on_message_received(Network::Message message);

    private:
        /*!
         * \brief Функция регистрации нового пользователя
         * \param nickname Никнейм нового пользователя
         * \param email email нового пользователя
         * \param password Пароль нового пользователя
         * \param client Клиент, который отправил запрос на регистрацию
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
        void create_user(QString nickname, QString email, QString password, Client client, Network::MessageIdType requestId);

        /*!
         * \brief Функция отправки сообщения в чат
         * \param sender id пользователя, который отправляет сообщение
         * \param toChat id чата, в который этот пользователь отправляет сообщение
         * \param data Сам текст сообщения
         * \param answerFor Список id сообщений, на которые отвечает данное сообщение
         * \param feature Дополнительный параметр сообщения
         */
        void send_chat_message(Database::UserIdType sender, Database::ChatIdType toChat, QString data, QList<Database::MessageIdType> answerFor,
                               Database::ChatFeature feature);
        /*!
         * \brief Функция входа пользователя в систему
         * \param email email под которым зарегистрирован пользователь
         * \param password Пароль, который ввел пользователь
         * \param client Клиент, который отправил этот запрос
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
        void login_user(QString email, QString password, Client client, Network::MessageIdType requestId);

        /*!
         * \brief Функция создания нового чата
         * (создает чат, добавляет туда создателя, назначает его админом и присылает ему ответом на его запрос создания id нового чата)
         * \param creator id пользователя, который хочет создать чат
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
        void create_chat(Database::UserIdType creator, Network::MessageIdType requestId);

        /*!
         * \brief Функция получения последних сообщений чата.
         * Проверяет, может ли пользователь получить эти сообщения (по его праву доступа) и высылает пользователю сообщение со всеми сообщениями
         * \param user id пользователя, который хочет получить сообщения
         * \param chat id чата, из которого хочет получить сообщения пользователь
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         * \param messagesCount Количество сообщений, которые следует загрузить (если не задавать, то будет использовано стандартное значение,
         * указанное в классе для работы с БД \ref Mafia::Database::ChatsDatabaseManager::MESSAGES_TO_UPLOAD_DEFAULT)
         */
        void get_last_messages(Database::UserIdType user, Database::ChatIdType chat, Network::MessageIdType requestId, int messagesCount = -1);

        /*!
         * \brief Функция получения всех чатов данного пользователя
         * Собирает все чаты пользователя, сортирует их в порядке убывания даты последнего сообщения в этом чате (как в ВК)
         * и отправляет пользователю
         * \param user id пользователя, который хочет получить список своих чатов
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         * \param chatsCount Количество чатов, которые следует получить
         * (если у пользователя меньше чатов, чем указанно, то будет отправлен меньший список)
         * (если оставить пустым, то будет использовано стандартное значение \ref Mafia::MainServerManager::CHATS_UPLOAD_COUNT)
         */
        void get_users_chats(Database::UserIdType user, Network::MessageIdType requestId, int chatsCount = CHATS_UPLOAD_COUNT);

        /*!
         * \brief Функция добавления пользователя в чат
         * Проверяет, может ли initializer по своим правам добавлять пользователя в этот чат и нет ли этого пользователя уже в этом чате,
         * добавляет пользователя в чат и назначает ему необходимую роль (права)
         * \param chat id чата, в который следует добавить пользователя
         * \param user id пользователя, которог следует добавить
         * \param initializer id пользователя, который хочет провести эту операцию
         * \param capability Роль (права) нового пользователя в этом чате
         */
        void add_user_to_chat(Database::ChatIdType chat, Database::UserIdType user, Database::UserIdType initializer,
                              Database::ChatCapability capability = Database::ChatCapabilities_Speaker);

        /*!
         * \brief Функция удаления пользователя из чата.
         * Проверяет, может ли initializer по своим правам выполнить такую операцию, проверяет, есть ли пользователь в чате и удаляет его из чата
         * \param chat id чата, из которого следует удалить пользователя
         * \param user id пользователя, которого следует удалить
         * \param initializer id пользователя, который хочет удалить другого пользователя
         */
        void remove_user_from_chat(Database::ChatIdType chat, Database::UserIdType user, Database::UserIdType initializer);

        /*!
         * \brief Функция изменения роли (прав) пользователя в некотором чате
         * Проверяет, может ли initializer изменить права пользователя, проверяет, есть ли пользователь в этом чате и изменяет права пользователя
         * \param chat id чата, в котором следует изменить права пользователя
         * \param user id пользователя, чьи права следует изменить
         * \param newCapability Новый статус прав пользователя
         * \param initializer id пользователя, который хочет изменить права другого пользователя
         */
        void change_users_chat_capability(Database::ChatIdType chat, Database::UserIdType user,
                                          Database::ChatCapability newCapability, Database::UserIdType initializer);

        /*!
         * \brief Функция создания (начала) новой игры
         * \param creator клиент, который хочет создать игру
         */
        void create_game(Client creator);

        /*!
         * \brief Функция получения статистики данного пользователя
         * \param user id пользователя, чьи данные требуются
         * \param asker id пользователя, который хочет получить эти данные
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
        void get_statistics(Database::UserIdType user, Database::UserIdType asker, Network::MessageIdType requestId);

        /*!
         * \brief Функция добавления в БД с играми только что завершившейся игры
         * \param game все данные об этой игре
         */
        void add_game(Gameplay::Game game);

        /*!
         * \brief Функция выхода пользователя из системы
         * \param user id пользователя, который хочет выйти из системы
         */
        void logout_user(Database::UserIdType user);

        /*!
         * \brief Функция получения и записи в файл с логами логов пользователя, который только что подключился к системе
         * Когда пользователь подключается к системе, он отправляет серверу все свои прошлые логи, которые накопились за прошлую сессию игры
         * Сервер получает эти логи и сохраняет их в свой файл
         * \param data Данные логов
         * \param sender Клиент, который их отправил
         */
        void get_logs_data(QString data, Client sender);

        /*!
         * \brief Функция добавления новой транзакции в БД с транзакциями и соответствующих изменений для пользователя, который заплатил
         * \param transaction Все данные о прошедшей транзакции
         */
        void add_transaction(Database::Transaction transaction);

        /*!
         * \brief Функция изменения никнейма пользователя
         * \param user id пользователя, который хочет изменить свой никнейм
         * \param newNickname Новый никнейм пользователя
         */
        void change_nickname(Database::UserIdType user, QString newNickname);

        /*!
         * \brief Функция изменения email-a пользователя
         * Функция пробует изменить email пользователя и отправляет пользователю ответ на его запрос - булевую переменную, отвечающую на вопрос:
         * "Удалось ли сменить email?"
         * \param user id пользователя, который хочет изменить свой email
         * \param newEmail Новый email
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
        void change_email(Database::UserIdType user, QString newEmail, Network::MessageIdType requestId);

        /*!
         * \brief Функция изменения достижения (нашивки) пользователя
         * Функция меняет нашивку пользователя и если он в этот момент онлайн, то отправляет ему сообщение с оповещением
         * \param user id пользователя, у которого следует изменить нашивку
         * \param achievement Новая нашивка
         */
        void change_achievement(Database::UserIdType user, Database::Achievement achievement);

        /*!
         * \brief Функция добавления пользователя в существующую игру
         * \param client Клиент, которого следует добавить
         * \param gameId id игры, в которую следует добавить пользователя
         */
        void add_user_to_game(Client client, int gameId);

        /*!
         * \brief Функция удаления сообщения из чата
         * Проверяет, из какого чата данное сообщение и может ли этот пользователь удалять сообщение в этом чате, и тогда удаляет его
         * \param user id пользователя, который хочет удалить сообщение
         * \param message id сообщения, которое хочет удалить пользователь
         */
        void delete_message(Database::UserIdType user, Database::MessageIdType message);

        /*!
         * \brief Функция изменения сообщения в чате
         * Проверяет, в каком чате это сообщение, может ли этот пользователь изменять это сообшение и если может, то пробует изменить
         * \param user id пользователя, который хочет изменить сообщение
         * \param message Все данные нового сообщения
         */
        void edit_message(Database::UserIdType user, Database::ChatMessage message);

        /*!
         * \brief Функция, оповещающая о том, что пользователь прочитал данное сообщение
         * Проверяет, мог ли этот пользователь прочитать это сообшение и если мог, то добавляет пользователя в список прочитавших
         * \param user id пользователя, который прочитал сообщение
         * \param message id сообщения, которое прочитал пользователь
         */
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

        /*!
         * \brief Функция, которая сопоставляет id пользователя, клиента, который сейчас зарегистрирован под этим id
         * \param user id пользователя
         * \return Все данные о клиенте, который зарегистрирован под этим id
         */
        Client get_client_by_user(Database::UserIdType user);

        /*!
         * \brief Функция, обратная \ref Mafia::MainServerManager::get_client_by_user(Database::UserIdType user)
         * Сопоставляет id пользователя, клиенту, который зарегистрирован под этим id в данный момент
         * \param client Клиент, который нас интересует
         * \return id пользователя, под которым этот клиент сейчас зарегистрирован
         */
        Database::UserIdType get_user_by_client(Client client);

        //! \brief Список всех клиентов, которые в данный момент онлайн
        MafiaList<Client> clients;

        //! \brief Список всех id пользователей, под которыми зарегистрировны клиенты (clients[i] зарегистрирован под users[i])
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
        //! \brief Индекс "никакого" пользователя - user id, который назначается неавторизированным пользователям
        static const Database::UserIdType nullUser = -1;
        //! \brief Стандартное количество чатов, которые следует загружать, если нет указаний по количеству чатов для загрузки
        static const int CHATS_UPLOAD_COUNT = 20;

    };
}

#endif // MAINSERVERMANAGER_H
