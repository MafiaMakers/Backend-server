#include "tst_transactiondatabasemanager.h"
#include "Exceptions/exception.h"

using namespace MafiaTests;
using namespace DatabaseTests;

TransactionDatabaseManagerTests::TransactionDatabaseManagerTests(QObject *parent) : QObject(parent)
{
	dbWorker = new Mafia::Database::DatabaseWorker();
	manager = new Mafia::Database::TransactionDatabaseManager(dbWorker);

	currentTransactions = MafiaList<Mafia::Database::Transaction>();
}

void TransactionDatabaseManagerTests::add_random_transaction()
{
	Mafia::Database::UserIdType userId = qrand();
	Mafia::Database::PriceType price = qrand();
	Mafia::Database::TransactionIdType transactionId = manager->add_transaction(userId, price,
		Mafia::Database::TransactionType_BuySomething);
	Mafia::Database::Transaction current = manager->get_transaction(transactionId);
	currentTransactions.append(current);
}

void TransactionDatabaseManagerTests::get_transaction_test()
{
	Mafia::Database::Transaction current = manager->get_transaction(currentTransactions[0].id);
	QCOMPARE(current, currentTransactions[0]);
}

void TransactionDatabaseManagerTests::get_transaction_incorrect_test()
{
	try {
		manager->get_transaction(456);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown
	}
}

void TransactionDatabaseManagerTests::add_more_transactions()
{
	treshold = QDateTime::currentDateTimeUtc();

	for(int i = 0; i < 3; i++){
		add_random_transaction();
	}
}

void TransactionDatabaseManagerTests::get_all_transactions_test()
{
	MafiaList<Mafia::Database::Transaction> allTransactions = manager->get_all_transactions();

	for(int i = 0; i < allTransactions.length(); i++){
		QVERIFY(currentTransactions.contains(allTransactions[i]));
	}
}

void TransactionDatabaseManagerTests::get_users_transactions_test()
{
	Mafia::Database::UserIdType user = currentTransactions[0].buyer;

	MafiaList<Mafia::Database::Transaction> usersTransactions = manager->get_users_transactions(user);

	for(int i = 0; i < currentTransactions.length(); i++){
		if(currentTransactions[i].buyer == user){
			QVERIFY(usersTransactions.contains(currentTransactions[i]));
		}
	}

	for(int i = 0; i < usersTransactions.length(); i++){
		if(usersTransactions[i].buyer == user){
			QVERIFY(currentTransactions.contains(usersTransactions[i]));
		}
	}
}

void TransactionDatabaseManagerTests::get_time_bounded_transactions_test()
{
	try {
		MafiaList<Mafia::Database::Transaction> afterTresholdTransactions = manager->get_time_bounded_transactions(treshold);

		for(int i = 0; i < currentTransactions.length(); i++){
			if(currentTransactions[i].timestamp > treshold){
				QVERIFY(afterTresholdTransactions.contains(currentTransactions[i]));
			}
		}

		for(int i = 0; i < afterTresholdTransactions.length(); i++){
			if(afterTresholdTransactions[i].timestamp > treshold){
				QVERIFY(currentTransactions.contains(afterTresholdTransactions[i]));
			}
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}
