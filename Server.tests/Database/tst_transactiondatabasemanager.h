#ifndef TRANSACIOTNDATABASEMANAGERTESTS_H
#define TRANSACIOTNDATABASEMANAGERTESTS_H
#include <QTest>
#include <QObject>
#include "Database/transactiondatabasemanager.h"

namespace MafiaTests {
	namespace DatabaseTests {
		class TransactionDatabaseManagerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit TransactionDatabaseManagerTests(QObject *parent = nullptr);

		private slots:
			void add_random_transaction();
			void get_transaction_test();
			void get_transaction_incorrect_test();
			void add_more_transactions();
			void get_all_transactions_test();
			void get_users_transactions_test();
			void get_time_bounded_transactions_test();

		private:
			Mafia::Database::DatabaseWorker* dbWorker;
			Mafia::Database::TransactionDatabaseManager* manager;

			MafiaList<Mafia::Database::Transaction> currentTransactions;
			QDateTime treshold;

		};
	}
}

#endif // TRANSACIOTNDATABASEMANAGERTESTS_H
