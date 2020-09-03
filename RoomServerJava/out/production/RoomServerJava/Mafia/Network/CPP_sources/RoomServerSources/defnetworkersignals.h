#ifndef DEFNETWORKERSIGNALS_H
#define DEFNETWORKERSIGNALS_H
#include "Network/mainservernetworker.h"
#include "jnimethodrunner.h"
using namespace Mafia;
using namespace Network;

void MainServerNetworker::on_subserver_api_message_received(Message message){
	System::JNIMethodRunner::run_subservernetworker_processmessage(message);
	std::cout << "Subserver api message received\n";
}

void MainServerNetworker::request_answer(Message message){
	System::JNIMethodRunner::run_subservernetworker_processmessage(message);
	std::cout << "Request answer received\n";
}

void MainServerNetworker::message_received(Message message){
	System::JNIMethodRunner::run_subservernetworker_processmessage(message);
	std::cout << "Message received\n";
}


#endif // DEFNETWORKERSIGNALS_H
