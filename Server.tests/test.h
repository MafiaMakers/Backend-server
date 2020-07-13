#ifndef TEST_H
#define TEST_H

#include <QTest>

int totalTestsFailed = 0;

template<class T>
void ADD_TO_TEST(int argc, char* argv[]) {
	T tc;
	totalTestsFailed += QTest::qExec(&tc, argc, argv);
}
#endif // TEST_H
