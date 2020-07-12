#ifndef SIMPLESTRINGTEST_H
#define SIMPLESTRINGTEST_H

#include <QTest>
#include <QObject>

namespace MafiaTests {
	namespace SystemTests {
		class SimpleStringTests : public QObject
		{
			Q_OBJECT
		public:
			explicit SimpleStringTests(QObject *parent = nullptr);

		private slots:
			void paste_correct();
			void paste_overflow();
		};
	}
}


#endif // SIMPLESTRINGTEST_H
