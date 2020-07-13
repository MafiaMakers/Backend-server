#include "tst_keygen.h"
#include "System/keygen.h"

using namespace MafiaTests;
using namespace SystemTests;

KeyGenTests::KeyGenTests(QObject *parent) : QObject(parent)
{

}

void KeyGenTests::short_string_test_alpha_chars()
{
	std::string key = Mafia::System::KeyGen::generate_key<std::string>(8);

	for(unsigned int i = 0; i < key.length(); i++){
		QVERIFY( is_alpha( key[ i ] ) );
	}
}

void KeyGenTests::long_string_test_alpha_chars()
{
	std::string key = Mafia::System::KeyGen::generate_key<std::string>(256);

	for(unsigned int i = 0; i < key.length(); i++){
		QVERIFY( is_alpha( key[ i ] ) );
	}
}

bool KeyGenTests::is_alpha(char letter)
{
	return (letter >= 'A' && letter <= 'Z');
}
