#include "System/functions.h"
#include "tst_functions.h"

using namespace MafiaTests;
using namespace SystemTests;

FunctionsTests::FunctionsTests(QObject *parent) : QObject(parent)
{
	currentInt = 0;
	currentArray = 0;
}

void FunctionsTests::allocate_int()
{
	SAFE_NEW(currentInt, int(0));
	QVERIFY(currentInt != 0);
}

void FunctionsTests::free_int()
{
	SAFE_DELETE(currentInt);
}

void FunctionsTests::allocate_array()
{
	SAFE_NEW(currentArray, int[8]);
	QVERIFY(currentArray != 0);
}

void FunctionsTests::delete_array()
{
	SAFE_DELETE(currentArray);
}
