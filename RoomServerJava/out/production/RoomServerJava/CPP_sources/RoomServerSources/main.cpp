#include "Main.h"
#include "Summator.h"
#include "Database/user.h"
#include <iostream>

JNIEXPORT jint JNICALL Java_Main_sum
  (JNIEnv *, jobject, jint a, jint b) {
    return Summator::sum(a, b);
}

JNIEXPORT jint JNICALL Java_Main_multiply
(JNIEnv* env, jobject obj, jint a, jint b) {
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "var", "I");
    jint var = env->GetIntField(obj, fid);

    jint nvar = var * 2;
    env->SetIntField(obj, fid, nvar);
    return Summator::multiply(a * var, b);
}

JNIEXPORT jstring JNICALL Java_Main_stringFunc
(JNIEnv* env, jobject obj, jstring str) {
    jstring nstr = env->NewStringUTF("Hello world!");
    int len = env->GetStringLength(str);
    jboolean isCopy = JNI_TRUE;
    char* chars = (char*)env->GetStringUTFChars(str, &isCopy);
    std::cout << "Source string data: " << chars << std::endl;
    return nstr;
}
