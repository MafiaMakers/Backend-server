#include "jnimethodrunner.h"
#include "stdarg.h"
#include <iostream>

using namespace Mafia;
using namespace System;

JNIEnv * JNIMethodRunner::environment = NULL;

void JNIMethodRunner::setup(JNIEnv* env)
{
	environment = env;
}

void JNIMethodRunner::run_messagesender_send(char* data, int size, int ip, int port)
{
	//std::cout << "Starting jni method\n";

	jbyteArray messageData = environment->NewByteArray(size);
	jbyte* messageData_data = environment->GetByteArrayElements(messageData, 0);
	//std::cout << "array created\n";
	for(int i = 0; i < size; i++){
		messageData_data[i] = data[i];
	}
	environment->ReleaseByteArrayElements(messageData, messageData_data, 0);

	//std::cout << "starting method\n";

	jclass cls = environment->FindClass("Mafia/Network/MessageSender");
	//std::cout << "cls: " << cls << "\n";
	jmethodID methodID = environment->GetStaticMethodID(cls, "send", "([BIII)V");
	//std::cout << "methodID: " << methodID << "\n";
	environment->CallStaticVoidMethod(cls, methodID, messageData, size, ip, port);
	//std::cout << "successfully finished!!!\n";
	/*run_static_method("Mafia/Network/MessageSender", "send", "([BIII)V",
					  messageData, size, ip, port);*/
}

void JNIMethodRunner::run_subservernetworker_processmessage(Network::Message message)
{
	std::cout << "RUN_SUBSERVER... started!!!\n";
	Network::Message* nMessage = new Network::Message();
	nMessage->id = message.id;
	nMessage->size = message.size;
	nMessage->type = message.type;
	nMessage->client = message.client;
	nMessage->partIndex = message.partIndex;
	nMessage->partsCount = message.partsCount;
	nMessage->data = new char[message.size];
	for(int i = 0; i < message.size; i++){
		nMessage->data[i] = message.data[i];
	}
	jlong messagePtr = (__int64)nMessage;

	std::cout << "Message is: " << nMessage->id << "\n" << nMessage->size << "\n" << nMessage->type << "\n"
			  << nMessage->client.ip << "\n" << nMessage->client.port << "\n" << nMessage->partIndex << "\n"
			  << nMessage->partsCount << "\n" << nMessage->data << "\n";

	jclass messageClass = environment->FindClass("Mafia/Network/MessageJ");
	std::cout << "messageClass: " << messageClass << "\n";
	jmethodID messageConstructor = environment->GetMethodID(messageClass, "<init>", "(J)V");
	std::cout << "messageConstructor: " << messageConstructor << "\n";
	jobject messageObj = environment->NewObject(messageClass, messageConstructor, messagePtr);
	std::cout << "messageObj: " << messageObj << "\n";

	jint id = environment->CallIntMethod(messageObj, environment->GetMethodID(messageClass, "Getid", "()I"));
	std::cout << id << " --- this is ID\n";
	jclass cls = environment->FindClass("Mafia/Network/SubserverNetworker");
	std::cout << "cls: " << cls << std::endl;
	jmethodID methodID = environment->GetStaticMethodID(cls, "process_message", "(LMafia/Network/MessageJ;)V");
	std::cout << "methodID: " << methodID << std::endl;
	environment->CallStaticVoidMethod(cls, methodID, messageObj);
	std::cerr << "finished\n";
	//run_static_method("Mafia/Network/SubserverNetworker", "process_message", "(LMafia/Network/MessageJ;)V", messageObj);
}

/*void JNIMethodRunner::run_static_method(const char* className, const char* methodName, const char* signature, ...)
{
	va_list ap;
	jclass cls = environment->FindClass(className);
	jmethodID id = environment->GetStaticMethodID(cls, methodName, signature);
	std::cout << "cls: " << cls << "\nid: " << id << "\nrunning...\n";
	va_start(ap, id);
	environment->CallStaticVoidMethod(cls, id, ap);
	va_end(ap);
	std::cout << "finished\n";
}*/

JNIMethodRunner::JNIMethodRunner(){}
