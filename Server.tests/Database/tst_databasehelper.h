#ifndef DATABASEHELPERTESTS_H
#define DATABASEHELPERTESTS_H
#include <QTest>
#include <QObject>

namespace MafiaTests{
	namespace DatabaseTests {
		class DatabaseHelperTests : public QObject
		{
			Q_OBJECT
		public:
			explicit DatabaseHelperTests(QObject *parent = nullptr);

		private slots:
			void int_test();
			void bool_test();
			void incorrect_test();
		private:
			static const int ARRAY_SIZE = 10;
		};
	}
}


#endif // DATABASEHELPERTESTS_H
