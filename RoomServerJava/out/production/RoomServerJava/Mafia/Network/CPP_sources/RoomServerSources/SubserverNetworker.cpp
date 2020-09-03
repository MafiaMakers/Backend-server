#ifndef _Included_Mafia_Network_SubserverNetworker
#define _Included_Mafia_Network_SubserverNetworker
#include <jni.h>
#include "Network/mainservernetworker.h"
#include "jnimethodrunner.h"
#include <iostream>

using namespace Mafia;
using namespace Network;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_Mafia_Network_SubserverNetworker_init
(JNIEnv * env, jclass networkerClass, jint port) {
	System::JNIMethodRunner::setup(env);
	jclass senderClass = env->FindClass("Mafia/Network/MessageSender");
	jmethodID senderInit = env->GetStaticMethodID(senderClass, "init", "(I)V");
	//std::cout << "PORT : " << port << std::endl;
	env->CallStaticVoidMethod(senderClass, senderInit, port);

	jfieldID pointerID = env->GetStaticFieldID(networkerClass, "networkerPointer", "J");
	MainServerNetworker * networker = new MainServerNetworker(port);
	//std::cout << "Got all data\n";
	//std::cout << "class: " << networkerClass << "\nid: " << pointerID << "\nnetworker: " << networker << "\n";
	env->SetStaticLongField(networkerClass, pointerID, (__int64)networker);
	//std::cout << "TEST\n";
	//std::cout << "Success!\n";
}

JNIEXPORT jint JNICALL Java_Mafia_Network_SubserverNetworker_sendMessage
(JNIEnv * env, jclass subNetClass, jobject message){
	//std::cout << "subNetClass: " << subNetClass << "\nMessage: " << message << "\n";
	jclass messageClass = env->GetObjectClass(message);
	//std::cout << "message class: " << messageClass << "\n";
	jfieldID ptrID = env->GetFieldID(messageClass, "_pointer", "J");
	//std::cout << "ptrID: "<< ptrID << std::endl;
	jlong messagePtr = env->GetLongField(message, ptrID);
	Message * toSend = (Message*)(__int64)messagePtr;
	jfieldID networkerPtrID = env->GetStaticFieldID(subNetClass, "networkerPointer", "J");
	//std::cout << "msgClass: " << messageClass << "\nptrID: " << ptrID << "\nmessagePtr: " << messagePtr <<
	///			 "\nsubNetClass: " << subNetClass << "\nNetworkerPtrID: " << networkerPtrID << "\n";
	jlong networkerPtr = env->GetStaticLongField(subNetClass, networkerPtrID);
	//std::cout << "networkerPtr found: " << networkerPtr << "\n";
	MainServerNetworker* networker = (MainServerNetworker*)(__int64)networkerPtr;
	//std::cout << "sending message\n";
	return networker->send_message(*toSend);
}

JNIEXPORT void JNICALL Java_Mafia_Network_SubserverNetworker_processReceivedBytes
(JNIEnv * env, jclass cls, jbyteArray arr, jint size, jint ip, jint port){
	std::cerr << "Processing moved to c++\n";
	jfieldID pointerID = env->GetStaticFieldID(cls, "networkerPointer", "J");
	//std::cerr << "pointerID : " << pointerID << "\n";
	jlong ptr = env->GetStaticLongField(cls, pointerID);
	//std::cerr << "ptr: " << ptr << "\n";
	MainServerNetworker * networker = (MainServerNetworker*)(__int64)ptr;
	std::cerr << "size: " << size << "\n";
	char* data;
	SAFE_NEW(data, char[size]);
	jbyte* arrData = env->GetByteArrayElements(arr, 0);
	//std::cerr << "OK\n";
	for(int i = 0; i < size; i++){
		//std::cerr << "writing " << i << "-th byte\n";
		data[i] = arrData[i];
	}
	//std::cout << "releasing bytes\n";
	env->ReleaseByteArrayElements(arr, arrData, 0);
	std::cerr << networker << "\nStarted networker processing\n";
	networker->receive_message(data, size, ip, port);
	std::cout << "successfully finished networker processing\n";
}

#ifdef __cplusplus
}
#endif
#endif
