#ifndef DATABASEWORKERTESTS_H
#define DATABASEWORKERTESTS_H
#include <QTest>
#include <QObject>
#include "Database/databaseworker.h"

namespace MafiaTests {
	namespace DatabaseTests {
		class DatabaseWorkerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit DatabaseWorkerTests(QObject *parent = nullptr);

		private slots:
			void init_test_ready_check();
			void run_query_test();
			void run_incorrect_query();
			void restore_ready();

		private:
			Mafia::Database::DatabaseWorker* worker;

		};
	}
}


#endif // DATABASEWORKERTESTS_H
