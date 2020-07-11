#include "System/tst_Tuple.h"
#include "System/tst_serializer.h"
#include <iostream>

using namespace MafiaTests::SystemTests;

int totalTestsFailed = 0;

#define ADD_TO_TEST(CLASS_NAME) {\
        TESTLIB_SELFCOVERAGE_START(CLASS_NAME)\
        CLASS_NAME tc;\
        totalTestsFailed += QTest::qExec(&tc, argc, argv);\
    }

int main(int argc, char* argv[]){
    QTEST_SET_MAIN_SOURCE_PATH

    ADD_TO_TEST(TupleTests)
    ADD_TO_TEST(SerializerTests)

    std::cout << "Total tests failed: " << totalTestsFailed << std::endl;
    return 0;
}
//#include "tst_Tuple.moc"
