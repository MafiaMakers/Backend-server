#include "tst_Tuple.h"

using namespace MafiaTests;
using namespace SystemTests;


TupleTests::TupleTests()
{

}

TupleTests::~TupleTests()
{

}

void TupleTests::int_int_tuple_equality_equal()
{
    Mafia::System::Tuple<int, int> first = Mafia::System::Tuple<int, int>(17, 179);
    Mafia::System::Tuple<int, int> second = Mafia::System::Tuple<int, int>(17, 179);

    bool areEqual = first == second;

    QCOMPARE(areEqual, true);
}

void TupleTests::int_int_tuple_equality_not_equal()
{
    Mafia::System::Tuple<int, int> first = Mafia::System::Tuple<int, int>(17, 179);
    Mafia::System::Tuple<int, int> second = Mafia::System::Tuple<int, int>(14, 179);

    bool areEqual = first == second;

    QCOMPARE(areEqual, false);
}

void TupleTests::qstring_double_equality_equal()
{
    Mafia::System::Tuple<QString, double> first = Mafia::System::Tuple<QString, double>("Hello world!", 3.1415926535);
    Mafia::System::Tuple<QString, double> second = Mafia::System::Tuple<QString, double>("Hello world!", 3.1415926535);

    bool areEqual = first == second;

    QCOMPARE(areEqual, true);
}

void TupleTests::qstring_double_equality_not_equal()
{
    Mafia::System::Tuple<QString, double> first = Mafia::System::Tuple<QString, double>("Hello world!", 3.1415926535);
    Mafia::System::Tuple<QString, double> second = Mafia::System::Tuple<QString, double>("Hello world!", 7.179);

    bool areEqual = first == second;

    QCOMPARE(areEqual, false);
}

void TupleTests::int_int_tuple_inequality_equal()
{
    Mafia::System::Tuple<int, int> first = Mafia::System::Tuple<int, int>(17, 179);
    Mafia::System::Tuple<int, int> second = Mafia::System::Tuple<int, int>(17, 179);

    bool areEqual = first != second;

    QCOMPARE(areEqual, false);
}

void TupleTests::int_int_tuple_inequality_not_equal()
{
    Mafia::System::Tuple<int, int> first = Mafia::System::Tuple<int, int>(17, 179);
    Mafia::System::Tuple<int, int> second = Mafia::System::Tuple<int, int>(14, 179);

    bool areEqual = first != second;

    QCOMPARE(areEqual, true);
}

void TupleTests::qstring_double_inequality_equal()
{
    Mafia::System::Tuple<QString, double> first = Mafia::System::Tuple<QString, double>("Hello world!", 3.1415926535);
    Mafia::System::Tuple<QString, double> second = Mafia::System::Tuple<QString, double>("Hello world!", 7.179);

    bool areEqual = first != second;

    QCOMPARE(areEqual, true);
}

void TupleTests::qstring_double_inequality_not_equal()
{
    Mafia::System::Tuple<QString, double> first = Mafia::System::Tuple<QString, double>("Hello world!", 7.179);
    Mafia::System::Tuple<QString, double> second = Mafia::System::Tuple<QString, double>("Hello world!", 7.179);

    bool areEqual = first != second;

    QCOMPARE(areEqual, false);
}


//QTEST_APPLESS_MAIN(MafiaTests::SystemTests::TupleTests)



