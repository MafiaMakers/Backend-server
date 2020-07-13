#ifndef LIMITEDQUEUETESTS_H
#define LIMITEDQUEUETESTS_H

#include "System/limitedqueue.h"
#include <QTest>
#include <QObject>

namespace MafiaTests {
	namespace SystemTests {
		class LimitedQueueTests : public QObject
		{
			Q_OBJECT
		public:
			explicit LimitedQueueTests(QObject *parent = nullptr);

		private slots:
			void empty_contains_test();
			void add_3_contains_3();
			void add_5_not_contains_7();
			void overflow_not_contains_3();
		private:
			Mafia::System::LimitedQueue<int> *queue;
			static const int limit = 16;

		};
	}
}


#endif // LIMITEDQUEUETESTS_H
