#include "tst_crypto.h"
#include "Network/crypto.h"
#include "Exceptions/exception.h"
#include "System/serializer.h"

using namespace MafiaTests;
using namespace NetworkTests;

CryptoTests::CryptoTests(QObject *parent) : QObject(parent)
{
	Mafia::Network::Crypto::set_key("some_key");
}

void CryptoTests::wrap_parse_text_message()
{
	Mafia::Network::Message messageToTest = Mafia::Network::Message(Mafia::Network::MessageType_Text,
		(Mafia::Network::SymbolType*)"This is some text.\nWith some aditional characters.\n\r",
		56,
		Mafia::Network::Client(QHostAddress("218.16.8.14").toIPv4Address(),
			179)
		);

	Mafia::System::String data = Mafia::Network::Crypto::wrap_message(messageToTest);

	Mafia::Network::Message resultMessage = Mafia::Network::Crypto::parse_data(data);

	QCOMPARE(messageToTest, resultMessage);
}

void CryptoTests::wrap_parse_notext_message()
{
	std::string testMessageData = Mafia::System::Serializer::serialize<Mafia::Network::Client>(
		Mafia::Network::Client(179, 789456));
	try {
		Mafia::Network::Message messageToTest = Mafia::Network::Message(Mafia::Network::MessageType_LoggedOut,
			(char*)testMessageData.c_str(),
			testMessageData.length(),
			Mafia::Network::Client(QHostAddress("11.05.79.73").toIPv4Address(),
				789)
			);

		Mafia::System::String data = Mafia::Network::Crypto::wrap_message(messageToTest);

		Mafia::Network::Message resultMessage = Mafia::Network::Crypto::parse_data(data);

		QCOMPARE(messageToTest, resultMessage);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QVERIFY(false);
	}
}

void CryptoTests::wrap_parse_empty_message()
{
	Mafia::Network::Message messageToTest = Mafia::Network::Message(Mafia::Network::MessageType_Text,
		(Mafia::Network::SymbolType*)"",
		0,
		Mafia::Network::Client(QHostAddress("111.11.1.111").toIPv4Address(),
			1111)
		);

	Mafia::System::String data = Mafia::Network::Crypto::wrap_message(messageToTest);

	Mafia::Network::Message resultMessage = Mafia::Network::Crypto::parse_data(data);

	QCOMPARE(messageToTest, resultMessage);
}

void CryptoTests::dich_key_test()
{
	Mafia::Network::Crypto::set_key(Mafia::System::String((char*)"\0\1\2\3\r\e\n\a", 8));
	try {
		wrap_parse_text_message();
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QVERIFY(false);
	}

}
