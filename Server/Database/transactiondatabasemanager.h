#ifndef TRANSACTIONDATABASEWORKER_H
#define TRANSACTIONDATABASEWORKER_H
#include "user.h"
#include "databasemanager.h"
namespace Mafia {
    namespace Database {
        //! \brief Основной класс для работы с БД транзакций
        class TransactionDatabaseManager : public DatabaseManager
        {
        public:
            /*!
             * \brief Основной конструктор, который создает, если надо, и связывается с БД и конкретной таблицей
             * \param databaseWorker Указатель на объект для работы с БД
             */
            TransactionDatabaseManager(DatabaseWorker* databaseWorker);

            /*!
             * \brief Функция добавления новой транзакции в БД
             * \param userId id платящего пользователя
             * \param price Цена, которую пользователь заплатил
             * \param type Тип транзакции
             * \return id добавленной транзакции
             */
            TransactionIdType add_transaction(UserIdType userId, PriceType price, TransactionType type);

            /*!
             * \brief Функция получения всех транзакций
             * \return Список из всех транзакций
             */
            MafiaList<Transaction> get_all_transactions();

            /*!
             * \brief Получение конкретной транзакции по id
             * \param id id транзакции
             * \return Вся информация о транзакции
             */
            Transaction get_transaction(TransactionIdType id);

            /*!
             * \brief Получение всех транзакций конкретного пользователя
             * \param buyer id пользователя
             * \return Список из всех транзакций
             */
            MafiaList<Transaction> get_users_transactions(UserIdType buyer);

            /*!
             * \brief Функция получения всех транзакций, проведенных в определенный временной интервал
             * \param after Будут возвращены транзакции, проведенные после этого момента
             * \param before Будут возвращены транзакции, проведенные до этого момента
             * \return Список всех транзакций, которые подходят под этот фильтр
             */
            MafiaList<Transaction> get_time_bounded_transactions(QDateTime after = BEGINNING_TIME, QDateTime before = ENDING_TIME);

        private:
            /*!
             * \brief Функция, которая запускается автоматически и каждые несколько секунд проверяет, не была ли взломана БД
             */
            void check_database_cracked();

            /*!
             * \brief Функция, которая проверяет хеши всех транзакций в БД
             * \return true, если хеши несовпадают. иначе - false
             */
            bool check_transaction_hashes();

            /*!
             * \brief Функция, вызываемая в случае обнаружения несовпадения хешей и, соответственно, взлома БД.
             * Пока что там просто вывод в консольку инфы
             */
            void on_database_cracked();

            /*!
             * \brief Функция добавления транзакции по всем значениям транзакции
             * \param transaction Все данные о транзакции
             * \return id добавленной транзакции (-1 в случае неудачи)
             */
            TransactionIdType add_transaction(Transaction transaction);

            /*!
             * \brief Получение списка всех транзакций из sql-запроса
             * \param query Sql-запрос с ответом
             * \return Список из всех транзакций, которые были получены на данный запрос
             */
            MafiaList<Transaction> get_request_transactions(QSqlQuery* query);

            //! \brief Временной интервал в мс через который проверяется БД на совпадение хешей
            static const int hashCheckingInterval;

        };
    }
}



#endif // TRANSACTIONDATABASEWORKER_H
