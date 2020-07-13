#include "tst_limitedqueue.h"

using namespace MafiaTests;
using namespace SystemTests;

LimitedQueueTests::LimitedQueueTests(QObject *parent) : QObject(parent)
{
	queue = new Mafia::System::LimitedQueue<int>(limit);
}

void LimitedQueueTests::empty_contains_test()
{
	bool contains = queue->contains(qrand());

	QVERIFY(contains == false);
}

void LimitedQueueTests::add_3_contains_3()
{
	queue->append(3);
	bool contains = queue->contains(3);

	QVERIFY(contains == true);
}

void LimitedQueueTests::add_5_not_contains_7()
{
	queue->append(5);
	bool contains = queue->contains(7);

	QVERIFY(contains == false);
}

void LimitedQueueTests::overflow_not_contains_3()
{
	for(int i = 1; i < limit; i++){
		queue->append(179);
	}

	bool contains = queue->contains(3);

	QVERIFY(contains == false);
}
