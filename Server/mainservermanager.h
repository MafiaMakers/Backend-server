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
/*! \mainpage Это документация к проекту основного сервера для онлайн игры в мафию
 *  Приблизительная архитектура: Всем процессом управляет \ref Mafia::MainServerManager.
 *  Для работы с БД он создает объекты классов из \ref Mafia::Database.
 *  Для работы с сетью он создает объект \ref Mafia::Network::MainServerNetworker.
 *  Также в \ref Mafia::Network есть \ref Mafia::Network::Message - структура для отправки и получения сообщений.
 *  А еще есть классы \ref Mafia::Network::MessageProcessing, которые отвечают за обработку полученных сообщений и вызов соответствующих слотов в
 *  \ref Mafia::MainServerManager.
 *  Все исключения, выбрасываемые функциями являются указателями на объект класса, наследованного от \ref Mafia::Exceptions::Exception.
 *  Также есть \ref Mafia::Gameplay - пространство имен для обработки объектов геймплея.
 *  Еще одна важная часть - система запросов (\ref Mafia::Requests). Запрос - это как функция, но ответ на нее может прийти очень нескоро,
 *  поэтому он отдельно инициализируется и отдельно из него получается значение. Запросы используются, например, для общения клиента с сервером,
 *  когда тот запрашивает какие-то данные из БД.
 *  Модуль \ref Mafia::Subservers отвечает за соединение с субсерверами. Каждый объект субсервера-класса связывается с конкретным запущенным
 *  приложением, отслеживает его работоспособность и обрабатывает приходящие от него сообщения и отправляемые ему.
 *  Последний модуль - \ref Mafia::System. В нем собраны полезные системные функции, такие как запуск и отслеживание программы, функция хеширования,
 *  сериализация, менеджмент интернет-портов и прочее (см. \ref Mafia::System).
 */


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

        //! \brief Функция, получающая все данные из БД и выводящая их в консоль
        void _database_get_all();

    private slots:
        /*!
         * \brief Слот, вызываемый, когда приходит новое сообщение в чат
         * \param message Сообщение
         */
        void on_chat_message_sent(Database::ChatMessage message);

        /*!
         * \brief Слот, вызываемый, когда какое-либо сообщение в чате изменено
         * \param message Измененное сообщение
         */
        void on_chat_message_edited(Database::ChatMessage message);

        /*!
         * \brief Слот, вызываемый, когда кто-либо прочитал какое-либо сообщение из какого-либо чата
         * \param message id сообщения, которое прочитали
         * \param chat Чат, из которого было это сообщение
         * \param user Пользователь, прочитавший сообщение
         */
        void on_chat_message_read(Database::MessageIdType message, Database::ChatIdType chat, Database::UserIdType user);

        /*!
         * \brief Слот, вызываемый при удалении сообщения из чата
         * \param messageId id удаляемого сообщения
         * \param chat Чат, из которого удалили сообщение
         */
        void on_chat_message_deleted(Database::MessageIdType messageId, Database::ChatIdType chat);

        /*!
         * \brief Функция регистрации нового пользователя
         * \param nickname Никнейм нового пользователя
         * \param email email нового пользователя
         * \param password Пароль нового пользователя
         * \param client Клиент, который отправил запрос на регистрацию
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
		void create_user(QString nickname, QString email, QString password, Network::Client client/*, Network::MessageIdType requestId*/);

        /*!
         * \brief Функция отправки сообщения в чат
         * \param sender клиент, который отправил сообщение
         * \param toChat id чата, в который этот пользователь отправляет сообщение
         * \param data Сам текст сообщения
         * \param answerFor Список id сообщений, на которые отвечает данное сообщение
         * \param feature Дополнительный параметр сообщения
         */
        void send_chat_message(Network::Client sender, Database::ChatIdType toChat, QString data, QList<Database::MessageIdType> answerFor,
                               Database::ChatFeature feature);
        /*!
         * \brief Функция входа пользователя в систему
         * \param email email под которым зарегистрирован пользователь
         * \param password Пароль, который ввел пользователь
         * \param client Клиент, который отправил этот запрос
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
		void login_user(QString email, QString password, Network::Client client/*, Network::MessageIdType requestId*/);

        /*!
         * \brief Функция создания нового чата
         * (создает чат, добавляет туда создателя, назначает его админом и присылает ему ответом на его запрос создания id нового чата)
         * \param creator Клиент, который хочет создать чат
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
		void create_chat(Network::Client creator/*, Network::MessageIdType requestId*/);

        /*!
         * \brief Функция получения последних сообщений чата.
         * Проверяет, может ли пользователь получить эти сообщения (по его праву доступа) и высылает пользователю сообщение со всеми сообщениями
         * \param client Клиент, который хочет получить сообщения
         * \param chat id чата, из которого хочет получить сообщения пользователь
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         * \param messagesCount Количество сообщений, которые следует загрузить (если не задавать, то будет использовано стандартное значение,
         * указанное в классе для работы с БД \ref Mafia::Database::ChatsDatabaseManager::MESSAGES_TO_UPLOAD_DEFAULT)
         */
		void get_last_messages(Network::Client client, Database::ChatIdType chat/*, Network::MessageIdType requestId*/, int messagesCount = -1);

        /*!
         * \brief Функция получения всех чатов данного пользователя
         * Собирает все чаты пользователя, сортирует их в порядке убывания даты последнего сообщения в этом чате (как в ВК)
         * и отправляет пользователю
         * \param client Клиент, который хочет получить список своих чатов
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         * \param chatsCount Количество чатов, которые следует получить
         * (если у пользователя меньше чатов, чем указанно, то будет отправлен меньший список)
         * (если оставить пустым, то будет использовано стандартное значение \ref Mafia::MainServerManager::CHATS_UPLOAD_COUNT)
         */
		void get_users_chats(Network::Client client/*, Network::MessageIdType requestId*/, int chatsCount = CHATS_UPLOAD_COUNT);

        /*!
         * \brief Функция добавления пользователя в чат
         * Проверяет, может ли initializer по своим правам добавлять пользователя в этот чат и нет ли этого пользователя уже в этом чате,
         * добавляет пользователя в чат и назначает ему необходимую роль (права)
         * \param chat id чата, в который следует добавить пользователя
         * \param user id пользователя, которог следует добавить
         * \param initializer id пользователя, который хочет провести эту операцию
         * \param capability Роль (права) нового пользователя в этом чате
         */
        void add_user_to_chat(Database::ChatIdType chat, Database::UserIdType user, Network::Client initializer,
                              Database::ChatCapability capability = Database::ChatCapabilities_Speaker);

        /*!
         * \brief Функция удаления пользователя из чата.
         * Проверяет, может ли initializer по своим правам выполнить такую операцию, проверяет, есть ли пользователь в чате и удаляет его из чата
         * \param chat id чата, из которого следует удалить пользователя
         * \param user id пользователя, которого следует удалить
         * \param initializer Клиент, который хочет добавить пользователя
         */
        void remove_user_from_chat(Database::ChatIdType chat, Database::UserIdType user, Network::Client initializer);

        /*!
         * \brief Функция изменения роли (прав) пользователя в некотором чате
         * Проверяет, может ли initializer изменить права пользователя, проверяет, есть ли пользователь в этом чате и изменяет права пользователя
         * \param chat id чата, в котором следует изменить права пользователя
         * \param user Клиент, который хочет добавить пользователя в чат
         * \param newCapability Новый статус прав пользователя
         * \param initializer id пользователя, который хочет изменить права другого пользователя
         */
        void change_users_chat_capability(Database::ChatIdType chat, Database::UserIdType user,
                                          Database::ChatCapability newCapability, Network::Client initializer);

        /*!
         * \brief Функция создания (начала) новой игры
         * \param creator клиент, который хочет создать игру
         */
        void create_game(Network::Client creator);

        /*!
         * \brief Функция получения статистики данного пользователя
         * \param user id пользователя, чьи данные требуются
         * \param asker Клиент, который хочет получить эти данные
         * \param requestId id запроса, который отправил клиент
         * (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
		void get_statistics(Database::UserIdType user, Network::Client asker/*, Network::MessageIdType requestId*/);

        /*!
         * \brief Функция добавления в БД с играми только что завершившейся игры
         * \param game все данные об этой игре
         */
        void add_game(Gameplay::Game game, Subservers::RoomSubserverObject* rso);

        /*!
         * \brief Слот, вызываемый, когда пользователь пытается подтвердить свой электронный адрес
         * \param client Клиент, который отправил запрос
         * \param confirmationKey Ключ, который передал клиент
         * \param requestId id запроса, который отправил клиент
         * (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
		void confirm_email(Network::Client client, QString confirmationKey/*, Network::MessageIdType requestId*/);

        /*!
         * \brief Функция выхода пользователя из системы
         * \param client Клиент, который хочет выйти из системы
         */
        void logout_user(Network::Client client);

        /*!
         * \brief Функция получения и записи в файл с логами логов пользователя, который только что подключился к системе
         * Когда пользователь подключается к системе, он отправляет серверу все свои прошлые логи, которые накопились за прошлую сессию игры
         * Сервер получает эти логи и сохраняет их в свой файл
         * \param data Данные логов
         * \param sender Клиент, который их отправил
         */
        void get_logs_data(QString data, Network::Client sender);

        /*!
         * \brief Функция добавления новой транзакции в БД с транзакциями и соответствующих изменений для пользователя, который заплатил
         * \param transaction Все данные о прошедшей транзакции
         */
        void add_transaction(Database::Transaction transaction);

        /*!
         * \brief Функция изменения никнейма пользователя
         * \param client Клиент, который хочет изменить свой никнейм
         * \param newNickname Новый никнейм пользователя
         */
        void change_nickname(Network::Client client, QString newNickname);

        /*!
         * \brief Функция изменения email-a пользователя
         * Функция пробует изменить email пользователя и отправляет пользователю ответ на его запрос - булевую переменную, отвечающую на вопрос:
         * "Удалось ли сменить email?"
         * \param client Клиент, который хочет изменить свой email
         * \param newEmail Новый email
         * \param requestId id запроса, который отправил клиент (id сообщения - для ответа на запрос (см. \ref Mafia::Requests::NetworkRequest))
         */
		void change_email(Network::Client client, QString newEmail/*, Network::MessageIdType requestId*/);

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
        void add_user_to_game(Network::Client client, QString gameKey);

        /*!
         * \brief Функция удаления сообщения из чата
         * Проверяет, из какого чата данное сообщение и может ли этот пользователь удалять сообщение в этом чате, и тогда удаляет его
         * \param client Клиент, который хочет удалить сообщение
         * \param message id сообщения, которое хочет удалить пользователь
         */
        void delete_message(Network::Client client, Database::MessageIdType message);

        /*!
         * \brief Функция изменения сообщения в чате
         * Проверяет, в каком чате это сообщение, может ли этот пользователь изменять это сообшение и если может, то пробует изменить
         * \param client Клиент, который хочет изменить сообщение
         * \param message Все данные нового сообщения
         */
        void edit_message(Network::Client client, Database::ChatMessage message);

        /*!
         * \brief Функция, оповещающая о том, что пользователь прочитал данное сообщение
         * Проверяет, мог ли этот пользователь прочитать это сообшение и если мог, то добавляет пользователя в список прочитавших
         * \param client Клиент, который прочитал сообщение
         * \param message id сообщения, которое прочитал пользователь
         */
        void read_message(Network::Client client, Database::MessageIdType message);

        private:
        /*!
         * \brief Функция, которая подключает слоты менеджера к сигналам Message Processor'а
         */
        void connect_to_processor();

        /*!
         * \brief Получает данные от уже инициализированного сетевого запроса
         * \param req Указатель на запрос
         */
		//void _get_data_from_request(Requests::NetworkRequest* req);

        /*!
         * \brief Функция, которая сопоставляет id пользователя, клиента, который сейчас зарегистрирован под этим id
         * \param user id пользователя
         * \return Все данные о клиенте, который зарегистрирован под этим id
         */
        Network::Client get_client_by_user(Database::UserIdType user);

        /*!
         * \brief Функция, обратная \ref Mafia::MainServerManager::get_client_by_user(Database::UserIdType user)
         * Сопоставляет id пользователя, клиенту, который зарегистрирован под этим id в данный момент
         * \param client Клиент, который нас интересует
         * \return id пользователя, под которым этот клиент сейчас зарегистрирован
         */
        Database::UserIdType get_user_by_client(Network::Client client);

		/*!
		 * \brief Функция отправки email-ов, оповещающих о проблеме на сервере
		 * \param message Сообщение, которое следует отправить
		 */
		void send_error_email(QString message);

		/*!
		 * \brief Если БД оказывается недоступна (какая-то фатальная ошибка),
		 * то вызывается эта функция, которая пытается устранить проблему.
		 * Если устранить проблему не удается, то она отправляет email-ы о проблеме
		 */
		void solve_database_error();

		/*!
		 * \brief Функция, которая решает проблему, возникающую при некорректном запросе пользователя.
		 * \param user Пользователь, который отправил некорректный запрос
		 */
		void solve_users_problem(Database::UserIdType user);

		/*!
		 * \brief Перегрузка вышеуказанной функции
		 * (\ref Mafia::MainServerManager::solve_users_problem(Database::UserIdType user))
		 * \param client Клиент, который отправил неверный запрос
		 */
		void solve_users_problem(Network::Client client);

		/*!
		 * \brief Функция стандартной обработки ошибки
		 * (если что либо выбрасывает ошибку,
		 * имеет смысл сначала применить стандартный обработчик,
		 * а затем уже уточнять дополнительные сведения)
		 * \param exception Исключение, которое было выброшено
		 */
		void standart_exception_processing(Exceptions::Exception* exception);

		/*!
		 * \brief Функция стандартной обработки исключений,
		 * которая учитывает пользователя,
		 * при работе с которым возникло исключение, и уведомляет его об этом
		 * \param exception Исключение, которое было выброшено
		 * \param user Пользователь, которого следует уведомить
		 */
		void standart_exception_processing(Exceptions::Exception* exception, Database::UserIdType user);

		/*!
		 * \brief Функция стандартной обработки исключений,
		 * которая учитывает клиента, при работе с которым произошла ошибка и уведомляет его.
		 * \param exception Исключение, которое было выброшено
		 * \param client Клиент, которого следует уведомить
		 */
		void standart_exception_processing(Exceptions::Exception* exception, Network::Client client);

		/*!
		 * \brief Функция, которая проверяет, не пустой ли пользователь (см. \ref Mafia::MainServerManager::nullUser)
		 * И если пользователь пустой, то выкидывает соответствующее исключение
		 * \param user Пользователь, которого следует проверить
		 */
		void check_null_user(Database::UserIdType user);

		/*!
		 * \brief Функция, которая высылает клиенту сообщение о том,
		 * что следует авторизоваться прежде чем выполнять действие,
		 * которое он хотел сделать
		 * \param client Клиент, которому следует выслать сообщение
		 */
		void send_unauthorized_exception(Network::Client client);

        //! \brief Список всех клиентов, которые в данный момент онлайн
        MafiaList<Network::Client> clients;

        //! \brief Список всех id пользователей, под которыми зарегистрировны клиенты (clients[i] зарегистрирован под users[i])
        MafiaList<Database::UserIdType> users;

        //! \brief Список всех субсерверов-комнат, которые сейчас активны
        MafiaList<Subservers::RoomSubserverObject*> games;

        //! \brief Указатель на объект субсервера, предназначенного для бэкапа сервера
        Subservers::BackupSubserverObject* backupSubserver;

        //! \brief Указатель на объект сетевого класса
		Network::Networker* networker;

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
		static const Database::UserIdType nullUser;
        //! \brief Стандартное количество чатов, которые следует загружать, если нет указаний по количеству чатов для загрузки
        static const int CHATS_UPLOAD_COUNT = 20;
		//! \brief Список email-ов людей, которые должны получать сообщения о неполадках сервера
		static const MafiaList<QString> errorEmailReceivers;

    };
}

#endif // MAINSERVERMANAGER_H
