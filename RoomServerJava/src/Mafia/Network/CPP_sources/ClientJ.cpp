#ifndef _Included_Mafia_Network_ClientJ
#define _Included_Mafia_Network_ClientJ
#include <jni.h>
#include "..\Mafia-server\Server\Network\message.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace Mafia;
using namespace Network;
JNIEXPORT void JNICALL Java_Mafia_Network_ClientJ_init
(JNIEnv * env, jobject obj) {
	Client * object = new Client();
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "_pointer", "J");
	env->SetLongField(obj, fid, (__int64)object);
}

Client * get_Client(JNIEnv * env, jobject obj) {
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "_pointer", "J");
	jlong var = env->GetLongField(obj, fid);
	Client* object = (Client*)(__int64)var;
	return object;
}
JNIEXPORT void JNICALL Java_Mafia_Network_ClientJ_finalize
(JNIEnv * env, jobject obj) {
	Client* object = get_Client(env, obj);
	if(object != 0) {
		delete object;
		object = 0;
	}
}
JNIEXPORT jboolean JNICALL Java_Mafia_Network_ClientJ_equals
(JNIEnv * env, jobject obj, jobject second) {
	Client* current = get_Client(env, obj);
	Client* another = get_Client(env, second);
	return (*current == *another);
}

JNIEXPORT jint JNICALL Java_Mafia_Network_ClientJ_Getip
(JNIEnv * env, jobject obj) {
	Client* object = get_Client(env, obj);
	int result = object->ip;
	return (jint)result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_ClientJ_Setip
(JNIEnv * env, jobject obj, jint data) {
	Client* object = get_Client(env, obj);
	object->ip = (int)data;
}

JNIEXPORT jint JNICALL Java_Mafia_Network_ClientJ_Getport
(JNIEnv * env, jobject obj) {
	Client* object = get_Client(env, obj);
	int result = object->port;
	return (jint)result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_ClientJ_Setport
(JNIEnv * env, jobject obj, jint data) {
	Client* object = get_Client(env, obj);
	object->port = (int)data;
}
#ifdef __cplusplus
}
#endif
#endif
