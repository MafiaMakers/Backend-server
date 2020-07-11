#include "tst_serializer.h"
#include "Exceptions/exception.h"

using namespace MafiaTests;
using namespace SystemTests;

SerializerTests::SerializerTests(QObject *parent) : QObject(parent)
{

}

SerializerTests::~SerializerTests()
{

}

void SerializerTests::int_11_11_returns()
{
    int toSerialize = 11;

    Mafia::System::String data = Mafia::System::String(Mafia::System::Serializer::serialize<int>(toSerialize));

    int result = Mafia::System::Serializer::deserialize<int>(data);

    QCOMPARE(toSerialize, result);
}

void SerializerTests::deserialize_short_message_int_error_returns()
{
    Mafia::System::String data = Mafia::System::String("12");

        QVERIFY_EXCEPTION_THROWN(Mafia::System::Serializer::deserialize<int>(data),
                                 Mafia::Exceptions::Exception*);

}
