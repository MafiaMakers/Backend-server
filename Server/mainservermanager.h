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

    private:
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

    };
}

#endif // MAINSERVERMANAGER_H
