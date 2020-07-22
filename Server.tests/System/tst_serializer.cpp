#include "tst_serializer.h"
#include "Database/chatsettingsdatabasemanager.h"
#include <MafiaExceptions>
#include "System/tuple.h"
#include "clientinfo.h"
#include "Network/message.h"

using namespace MafiaTests;
using namespace SystemTests;

SerializerTests::SerializerTests(QObject *parent) : QObject(parent)
{

}

SerializerTests::~SerializerTests()
{

}

void SerializerTests::integer()
{
    int toSerialize = qrand();

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

void SerializerTests::client()
{
    Mafia::Network::Client source(qrand(), qrand());

    Mafia::System::String data = Mafia::System::Serializer::serialize<decltype (source)>(source);
    Mafia::Network::Client result = Mafia::System::Serializer::deserialize<Mafia::Network::Client>(data);

    QCOMPARE(source.ip, result.ip);
    QCOMPARE(source.port, result.port);
}

void SerializerTests::chat_capability()
{
    Mafia::Database::ChatCapability source = Mafia::Database::ChatCapabilities_Admin;

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::Database::ChatCapability>(source);
    Mafia::Database::ChatCapability result = Mafia::System::Serializer::deserialize<Mafia::Database::ChatCapability>(data);

    QCOMPARE(source, result);
}

void SerializerTests::list_of_int()
{
    MafiaList<int> source = MafiaList<int>();

    int length = (qrand() % 128);

    for(int i = 0; i < length; i++){
        source.append(qrand());
    }

    Mafia::System::String data = Mafia::System::Serializer::serialize<MafiaList<int>>(source);
    MafiaList<int> result = Mafia::System::Serializer::deserialize<MafiaList<int>>(data);

    QCOMPARE(source, result);

}

void SerializerTests::character()
{
    char source = (char)(qrand() % 256);

    Mafia::System::String data = Mafia::System::Serializer::serialize<char>(source);
    char result = Mafia::System::Serializer::deserialize<char>(data);

    QCOMPARE(source, result);
}

void SerializerTests::boolean()
{
    bool source = (bool)(qrand() % 2);

    Mafia::System::String data = Mafia::System::Serializer::serialize<bool>(source);
    bool result = Mafia::System::Serializer::deserialize<bool>(data);

    QCOMPARE(source, result);
}

void SerializerTests::list_of_char()
{
    MafiaList<char> source = MafiaList<char>();

    int length = (qrand() % 128);

    for(int i = 0; i < length; i++){
        source.append((char)(qrand() % 256));
    }

    Mafia::System::String data = Mafia::System::Serializer::serialize<MafiaList<char>>(source);
    MafiaList<char> result = Mafia::System::Serializer::deserialize<MafiaList<char>>(data);

    QCOMPARE(source, result);
}

void SerializerTests::qstring()
{
    QString source = "Hel\0lo\n \rworld!!";

    Mafia::System::String data = Mafia::System::Serializer::serialize<QString>(source);
    QString result = Mafia::System::Serializer::deserialize<QString>(data);

    QCOMPARE(source, result);
}

void SerializerTests::qdatetime()
{
    QDateTime source = QDateTime::fromString("04.10.2003 01:01:01", Mafia::Database::SQL_DATETIME_FORMAT);

    Mafia::System::String data = Mafia::System::Serializer::serialize<QDateTime>(source);
    QDateTime result = Mafia::System::Serializer::deserialize<QDateTime>(data);

    QCOMPARE(source, result);
}

void SerializerTests::chat_message()
{
    Mafia::Database::ChatMessage source = generate_random_message();

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::Database::ChatMessage>(source);
    Mafia::Database::ChatMessage result = Mafia::System::Serializer::deserialize<Mafia::Database::ChatMessage>(data);

	QCOMPARE(source, result);
}

void SerializerTests::list_of_chat_message()
{
    auto source = MafiaList<Mafia::Database::ChatMessage>();

    int length = 64;
    for(int i = 0; i < length; i++){
        source.append(generate_random_message());
    }

    Mafia::System::String data = Mafia::System::Serializer::serialize<MafiaList<Mafia::Database::ChatMessage>>(source);
    MafiaList<Mafia::Database::ChatMessage> result =
            Mafia::System::Serializer::deserialize<MafiaList<Mafia::Database::ChatMessage>>(data);

    for( int i = 0; i < length; i++ ) {
		QCOMPARE( source[ i ], result[ i ] );
    }
}

void SerializerTests::user_statistics()
{
    Mafia::UserStatistics source = generate_random_statistics();

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::UserStatistics>(source);
    Mafia::UserStatistics result = Mafia::System::Serializer::deserialize<Mafia::UserStatistics>(data);

	QCOMPARE(source, result);
}

void SerializerTests::tuple_of_user_id_and_qstring()
{
    auto source = Mafia::System::Tuple<Mafia::Database::UserIdType, QString>();
    source.item1 = qrand();
    source.item2 = "Qwtyu";

    Mafia::System::String data =
            Mafia::System::Serializer::serialize<Mafia::System::Tuple<Mafia::Database::UserIdType, QString>>(source);
    auto result = Mafia::System::Serializer::deserialize<Mafia::System::Tuple<Mafia::Database::UserIdType, QString>>(data);

    QCOMPARE(source.item1, result.item1);
    QCOMPARE(source.item2, result.item2);
}

void SerializerTests::client_info()
{
    Mafia::ClientInfo source = Mafia::ClientInfo();
    source.client = Mafia::Network::Client(qrand(), qrand());
    source.statistics = generate_random_statistics();

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::ClientInfo>(source);
    auto result = Mafia::System::Serializer::deserialize<Mafia::ClientInfo>(data);

    QCOMPARE(source.client.ip, result.client.ip);
    QCOMPARE(source.client.port, result.client.port);
	QCOMPARE(source.statistics, result.statistics);

}

void SerializerTests::list_of_roles()
{
    auto source = MafiaList<Mafia::Gameplay::Role>();

    int length = 64;
    for(int i = 0; i < length; i++){
        source.append(Mafia::Gameplay::Role_Mafia);
    }

    Mafia::System::String data = Mafia::System::Serializer::serialize<MafiaList<Mafia::Gameplay::Role>>(source);
    auto result = Mafia::System::Serializer::deserialize<MafiaList<Mafia::Gameplay::Role>>(data);

    QCOMPARE(source, result);
}

void SerializerTests::game()
{
    auto source = Mafia::Gameplay::Game();
    source.id = qrand();
    source.roles = MafiaList<Mafia::Gameplay::Role>() << Mafia::Gameplay::Role_Mafia << Mafia::Gameplay::Role_Doctor;
    source.users = MafiaList<Mafia::Database::UserIdType>() << qrand() << qrand();
    source.result = Mafia::Gameplay::GameResult_MafiaWins;
    source.endingDT = QDateTime::fromString("14.05.2019 11:00:12", Mafia::Database::SQL_DATETIME_FORMAT);
    source.beginningDT = QDateTime::fromString("14.05.2019 12:00:12", Mafia::Database::SQL_DATETIME_FORMAT);

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::Gameplay::Game>(source);
    auto result = Mafia::System::Serializer::deserialize<Mafia::Gameplay::Game>(data);

	QCOMPARE(source, result);
}

void SerializerTests::tuple_of_int_and_int()
{
    auto source = Mafia::System::Tuple<int, int>(qrand(), qrand());

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::System::Tuple<int, int>>(source);
    auto result = Mafia::System::Serializer::deserialize<Mafia::System::Tuple<int, int>>(data);

    QCOMPARE(source.item1, result.item1);
    QCOMPARE(source.item2, result.item2);
}

void SerializerTests::network_message()
{
    Mafia::Network::Message source = generate_random_network_message();

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::Network::Message>(source);
    auto result = Mafia::System::Serializer::deserialize<Mafia::Network::Message>(data);

	QCOMPARE(source, result);
}

void SerializerTests::tuple_of_network_message_and_int()
{
    auto source = Mafia::System::Tuple<Mafia::Network::Message, int>();
    source.item1 = generate_random_network_message();
    source.item2 = qrand();

    Mafia::System::String data = Mafia::System::Serializer::serialize<Mafia::System::Tuple<Mafia::Network::Message, int>>(source);
    auto result = Mafia::System::Serializer::deserialize<Mafia::System::Tuple<Mafia::Network::Message, int>>(data);

	QCOMPARE(source.item1, result.item1);
    QCOMPARE(source.item2, result.item2);
}

Mafia::Database::ChatMessage SerializerTests::generate_random_message()
{
    auto result = Mafia::Database::ChatMessage();

    result.id = qrand();
    result.data = "Let's go out";
    result.from = qrand();
    result.toChat = qrand();
    result.feature = (Mafia::Database::ChatFeature)qrand();
    result.answerFor = MafiaList<Mafia::Database::MessageIdType>() << qrand() << qrand();
    result.readUsers = MafiaList<Mafia::Database::MessageIdType>() << qrand() << qrand() << qrand();
    result.timestamp = QDateTime::fromString("12.07.2020 16:19:15", Mafia::Database::SQL_DATETIME_FORMAT);

    return result;
}

Mafia::UserStatistics SerializerTests::generate_random_statistics()
{
    auto source = Mafia::UserStatistics();
    source.id = qrand();
    source.nickname = "UserQwerty";
    source.accountType = Mafia::Database::AccountType_Pro;
    source.achievement = Mafia::Database::Achievement_ProMLGPlayer;
    source.loginDateTime = QDateTime::fromString("26.07.2019 01:01:01", Mafia::Database::SQL_DATETIME_FORMAT);
    source.defeatesByRoles = MafiaList<int>() << qrand() << qrand() << qrand() << qrand() << qrand();
    source.victoriesByRoles = MafiaList<int>() << qrand() << qrand() << qrand() << qrand() << qrand();

    return source;
}

Mafia::Network::Message SerializerTests::generate_random_network_message()
{
    return Mafia::Network::Message(Mafia::Network::MessageType_Text,
                 (Mafia::Network::SymbolType*)"Qwertyasdfgggfda",
                 20,
                 Mafia::Network::Client(qrand(), qrand()),
                 qrand(), qrand(), qrand()
                 );
}
