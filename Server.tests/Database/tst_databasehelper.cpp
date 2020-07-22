#include "tst_databasehelper.h"
#include "Database/databasehelper.h"
#include "System/keygen.h"

using namespace MafiaTests;
using namespace DatabaseTests;

DatabaseHelperTests::DatabaseHelperTests(QObject *parent) : QObject(parent){}

void DatabaseHelperTests::int_test()
{
	MafiaList<int> source = MafiaList<int>();
	for( int i = 0; i < ARRAY_SIZE; i++ ){
		source.append( qrand() );
	}

	QByteArray data = Mafia::Database::qbytearray_from_qlist<int>(source);
	MafiaList<int> result = Mafia::Database::qlist_from_byte_array<int>(data);

	QCOMPARE(result, source);
}

void DatabaseHelperTests::bool_test()
{
	MafiaList<bool> source = MafiaList<bool>();
	for( int i = 0; i < ARRAY_SIZE; i++ ){
		source.append( (bool)(qrand() % 2));
	}

	QByteArray data = Mafia::Database::qbytearray_from_qlist<bool>(source);
	MafiaList<bool> result = Mafia::Database::qlist_from_byte_array<bool>(data);

	QCOMPARE(result, source);
}

void DatabaseHelperTests::incorrect_test()
{
	QByteArray incorrectData = QByteArray::fromStdString("qwerty");
	try {
		Mafia::Database::qlist_from_byte_array<int>(incorrectData);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Ok. Exception is thrown.
	}
}
