#ifndef FUNCTIONSTESTS_H
#define FUNCTIONSTESTS_H

#include <QTest>
#include <QObject>

namespace MafiaTests {
	namespace SystemTests {
		class FunctionsTests : public QObject
		{
			Q_OBJECT
		public:
			explicit FunctionsTests(QObject *parent = nullptr);

		private slots:
			void allocate_int();
			void free_int();
			void allocate_array();
			void delete_array();

		private:
			int* currentInt;
			int* currentArray;
		};
	}
}


#endif // FUNCTIONSTESTS_H
