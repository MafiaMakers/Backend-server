#include "tst_SHA_256.h"
#include "System/SHA_256.h"
using namespace MafiaTests;
using namespace SystemTests;

SHA256Tests::SHA256Tests(QObject *parent) : QObject(parent)
{

}

void SHA256Tests::different_hashes()
{
	std::string first = "Hello world!";
	std::string firstHash = Mafia::System::SHA256().hash(first);

	std::string second = "Goodbye world!";
	std::string secondHash = Mafia::System::SHA256().hash(second);

	bool equal = secondHash == firstHash;
	QCOMPARE(equal, false);
}

void SHA256Tests::equal_hashes()
{
	std::string first = "Hello world!";
	std::string firstHash = Mafia::System::SHA256().hash( first );

	std::string second = "Hello world!";
	std::string secondHash = Mafia::System::SHA256().hash( second );

	bool equal = secondHash == firstHash;
	QCOMPARE(equal, true);
}

void SHA256Tests::long_string_hash()
{
	int length = 1024;
	std::string data = "";
	for( int i = 0; i < length; i++ ){
		data += (char)( 'A' + ( qrand() % 26 ) );
	}

	std::string hash1 = Mafia::System::SHA256().hash( data );
	std::string hash2 = Mafia::System::SHA256().hash( data );

	QCOMPARE(hash1, hash2);
}
