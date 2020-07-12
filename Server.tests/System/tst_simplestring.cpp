#include "tst_simplestring.h"
#include "System/simplestring.h"

using namespace MafiaTests;
using namespace SystemTests;

SimpleStringTests::SimpleStringTests(QObject *parent) : QObject(parent)
{

}

void SimpleStringTests::paste_correct()
{
	Mafia::System::String buffer = Mafia::System::String("_________");
	Mafia::System::String source = Mafia::System::String("12345");

	buffer.paste(source, 2);

	QCOMPARE(buffer.size, 9);
	QCOMPARE(buffer.data, (char*)"__12345__");
}

void SimpleStringTests::paste_overflow()
{
	Mafia::System::String smallBuffer = Mafia::System::String("___");
	Mafia::System::String largeSource = Mafia::System::String("ochen' mnogo dannyh");

	smallBuffer.paste(largeSource);
	smallBuffer.data[smallBuffer.size - 1] = '\0';

	QCOMPARE(smallBuffer.size, 3);
	QCOMPARE(smallBuffer.data, (char*)"oc");
}
