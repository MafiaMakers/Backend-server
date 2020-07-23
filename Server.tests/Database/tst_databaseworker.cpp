#include "tst_databaseworker.h"
#include "Exceptions/exception.h"

using namespace MafiaTests;
using namespace DatabaseTests;

DatabaseWorkerTests::DatabaseWorkerTests(QObject *parent) : QObject(parent)
{
	worker = new Mafia::Database::DatabaseWorker();
}

void DatabaseWorkerTests::init_test_ready_check()
{
	QVERIFY(worker->database_open());
	QVERIFY(worker->database_ready());
}

void DatabaseWorkerTests::run_query_test()
{
	try {
		worker->run_query("CREATE TABLE TEST_TABLE (ID integer PRIMARY KEY NOT NULL, SOME integer)");
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void DatabaseWorkerTests::run_incorrect_query()
{
	try {
		worker->run_query("DICH request. Eto voobsche chto???");
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}

}

void DatabaseWorkerTests::restore_ready()
{
	worker->restore_database();
	init_test_ready_check();
}
