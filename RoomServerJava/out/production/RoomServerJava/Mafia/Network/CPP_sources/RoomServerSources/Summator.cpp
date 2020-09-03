#include "System/functions.h"
#include "System/tuple.h"
#include "System/keygen.h"
#include "Database/user.h"
#include "Database/chatmessage.h"
#include "Database/chatsettings.h"
#include "Exceptions/exception.h"
#include "clientinfo.h"
#include "userstatistics.h"
#include "Summator.h"
#include "System/serializer.h"
#include "Network/crypto.h"
#include "Network/message.h"
#include "Network/messageTypes.h"
#include "Network/mainservernetworker.h"

int Summator::sum(int a, int b){
	int* q = new int(1);
	REGISTER(q);
	SAFE_DELETE(q);

	Mafia::Network::MainServerNetworker* ntwrkr = new Mafia::Network::MainServerNetworker(15000);
	Mafia::Network::Crypto::set_key("testkey");
	ntwrkr->send_message(Mafia::Network::Message(
							 Mafia::Network::MessageType_Text,
							 (char*)"Hello, this is test message",
							 27,
							 Mafia::Network::Client(Mafia::Network::LOCALHOST_IP, 15000)));

	return a + b;
}

int Summator::multiply(int a, int b) {
	return a * b;
}
