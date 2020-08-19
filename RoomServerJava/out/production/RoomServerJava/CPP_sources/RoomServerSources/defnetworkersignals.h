#ifndef DEFNETWORKERSIGNALS_H
#define DEFNETWORKERSIGNALS_H
#include "Network/mainservernetworker.h"
using namespace Mafia;
using namespace Network;

void MainServerNetworker::on_subserver_api_message_received(Message message){
	std::cout << "Subserver api message received\n";
}

void MainServerNetworker::request_answer(Message message){
	std::cout << "Request answer received\n";
}

void MainServerNetworker::message_received(Message message){
	std::cout << "Message received\n";
}


#endif // DEFNETWORKERSIGNALS_H
