#ifndef _Included_Mafia_Network_MessageJ
#define _Included_Mafia_Network_MessageJ
#include <jni.h>
#include "..\Mafia-server\Server\Network\message.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace Mafia;
using namespace Network;
JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_init
(JNIEnv * env, jobject obj) {
	Message * object = new Message();
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "_pointer", "J");
	env->SetLongField(obj, fid, (__int64)object);
}

Message * get_Message(JNIEnv * env, jobject obj) {
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, "_pointer", "J");
	jlong var = env->GetLongField(obj, fid);
	Message* object = (Message*)(__int64)var;
	return object;
}
JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_finalize
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	if(object != 0) {
		delete object;
		object = 0;
	}
}
JNIEXPORT jboolean JNICALL Java_Mafia_Network_MessageJ_equals
(JNIEnv * env, jobject obj, jobject second) {
	Message* current = get_Message(env, obj);
	Message* another = get_Message(env, second);
	return (*current == *another);
}

JNIEXPORT jint JNICALL Java_Mafia_Network_MessageJ_Getid
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	MessageIdType result = object->id;
	return (jint)result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_Setid
(JNIEnv * env, jobject obj, jint data) {
	Message* object = get_Message(env, obj);
	object->id = (MessageIdType)data;
}

JNIEXPORT jint JNICALL Java_Mafia_Network_MessageJ_GetpartsCount
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	int result = object->partsCount;
	return (jint)result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_SetpartsCount
(JNIEnv * env, jobject obj, jint data) {
	Message* object = get_Message(env, obj);
	object->partsCount = (int)data;
}

JNIEXPORT jint JNICALL Java_Mafia_Network_MessageJ_GetpartIndex
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	int result = object->partIndex;
	return (jint)result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_SetpartIndex
(JNIEnv * env, jobject obj, jint data) {
	Message* object = get_Message(env, obj);
	object->partIndex = (int)data;
}

JNIEXPORT jint JNICALL Java_Mafia_Network_MessageJ_Gettype
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	MessageType result = object->type;
	return (jint)result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_Settype
(JNIEnv * env, jobject obj, jint data) {
	Message* object = get_Message(env, obj);
	object->type = (MessageType)data;
}

JNIEXPORT jobject JNICALL Java_Mafia_Network_MessageJ_Getclient
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	jclass cls = env->FindClass("Mafia/Network/ClientJ");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
	jobject result = env->NewObject(cls, constructor, (jlong)&(object->client));
	return result;
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_Setclient
(JNIEnv * env, jobject obj, jobject data) {
	Message* object = get_Message(env, obj);
	jclass cls = env->GetObjectClass(data);
	jfieldID fid = env->GetFieldID(cls, "_pointer", "J");
	jlong var = env->GetLongField(data, fid);
	Client* _data = (Client*)(__int64)var;
	object->client = *_data;
}

JNIEXPORT jcharArray JNICALL Java_Mafia_Network_MessageJ_Getdata
(JNIEnv * env, jobject obj) {
	Message* object = get_Message(env, obj);
	jcharArray arr = env->NewCharArray(object->size);
	jchar* jarr = env->GetCharArrayElements(arr, 0);
	for (int i; i < object->size; i++) {
		jarr[i] = object->data[i];
	}
	env->ReleaseCharArrayElements(arr, jarr, 0);
	return arr;
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_Setdata
(JNIEnv * env, jobject obj, jcharArray data) {
	Message* object = get_Message(env, obj);
	int size = env->GetArrayLength(data);
	object->data = new SymbolType[size];
	object->size = size;
	jchar* jarr = env->GetCharArrayElements(data, 0);
	for (int i = 0; i < size; i++) {
		object->data[i] = (SymbolType)jarr[i];
	}
}

JNIEXPORT void JNICALL Java_Mafia_Network_MessageJ_SetdataElement
(JNIEnv * env, jobject obj, jchar value, jint index) {
	Message* object = get_Message(env, obj);
	if (0 <= index && index < object->size) {
		object->data[index] = (SymbolType)value;
	} else {
		jclass exc_class = env->FindClass("java/lang/IndexOutOfBoundsException");
		env->ThrowNew(exc_class, "Index is negative or larger than the size of collection");
	}
}
#ifdef __cplusplus
}
#endif
#endif
