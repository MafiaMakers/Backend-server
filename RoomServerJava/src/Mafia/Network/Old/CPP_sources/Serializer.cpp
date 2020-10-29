#include "System/Serializer.h"
#include "Mafia_Network_Serializer.h"
#include "Mafia_Network_MessageParser.h"



jbyteArray JNICALL Java_Mafia_Network_Serializer_deserialize
(JNIEnv* env, jclass cls, jbyteArray data) {
	jclass strClass = env->FindClass("java/lang/String");
	//std::cerr << strClass << std::endl;

	jbyte* strBytes = env->GetByteArrayElements(data, 0);
	//std::cerr << strBytes << std::endl;
	int length = env->GetArrayLength(data);
	//std::cerr << length << std::endl;

	Mafia::System::String str = Mafia::System::String((char*)strBytes, length);
	env->ReleaseByteArrayElements(data, strBytes, 0);

	try {

		std::string res = Mafia::System::Serializer::deserialize<std::string>(str);
		//std::cerr << str.data << std::endl;
		jbyte* strBytes1 = env->GetByteArrayElements(data, 0);
		for (int i = 0; i < str.size; i++)
		{
			strBytes1[i] = str.data[i];
		}
		strBytes1[str.size + 1] = 0;
		env->ReleaseByteArrayElements(data, strBytes1, 0);

		jbyteArray arr = env->NewByteArray(res.length());
		jbyte* chars = env->GetByteArrayElements(arr, 0);
		//std::cerr << res << std::endl;

		for (int i = 0; i < res.length(); i++)
		{
			chars[i] = res[i];
		}
		env->ReleaseByteArrayElements(arr, chars, 0);

		//std::cerr << "finished\n";
		return arr;
	}
	catch (Mafia::Exceptions::Exception* ex) {
		raise_exception(env, ex);
		return 0;
	}
}

jbyteArray JNICALL Java_Mafia_Network_Serializer_serialize___3B
(JNIEnv* env, jclass cls, jbyteArray data) {
	jclass strClass = env->FindClass("java/lang/String");
	//std::cerr << strClass << std::endl;

	jbyte* strBytes = env->GetByteArrayElements(data, 0);
	//std::cerr << strBytes << std::endl;
	int length = env->GetArrayLength(data);
	//std::cerr << length << std::endl;
	Mafia::System::String str = Mafia::System::String((char*)strBytes, length);
	env->ReleaseByteArrayElements(data, strBytes, 0);

	try {
		Mafia::System::String res = Mafia::System::Serializer::serialize<std::string>(std::string(str.data, str.size));

		//std::cerr << res.data << std::endl;
		jbyteArray result = env->NewByteArray(res.size);
		//std::cerr << result << std::endl;
		jbyte* bytes = env->GetByteArrayElements(result, 0);

		for (int i = 0; i < res.size; i++)
		{
			bytes[i] = res.data[i];
		}

		env->ReleaseByteArrayElements(result, bytes, 0);
		//std::cerr << "finished\n";
		return result;
	}
	catch (Mafia::Exceptions::Exception* ex) {
		raise_exception(env, ex);
		return 0;
	}
}


jbyteArray JNICALL Java_Mafia_Network_Serializer_serialize__I
(JNIEnv* env, jclass cls, jint data) {

	try {
		Mafia::System::String res = Mafia::System::Serializer::serialize<int>((int)data);

		//std::cerr << res.data << std::endl;
		jbyteArray result = env->NewByteArray(res.size);
		//std::cerr << result << std::endl;
		jbyte* bytes = env->GetByteArrayElements(result, 0);

		for (int i = 0; i < res.size; i++)
		{
			bytes[i] = res.data[i];
		}

		env->ReleaseByteArrayElements(result, bytes, 0);
		//std::cerr << "finished\n";
		return result;
	}
	catch (Mafia::Exceptions::Exception* ex) {
		raise_exception(env, ex);
		return 0;
	}
}

jint JNICALL Java_Mafia_Network_Serializer_deserialize_1int
(JNIEnv* env, jclass cls, jbyteArray data) {
	jclass strClass = env->FindClass("java/lang/String");
	//std::cerr << strClass << std::endl;

	jbyte* strBytes = env->GetByteArrayElements(data, 0);
	//std::cerr << strBytes << std::endl;
	int length = env->GetArrayLength(data);
	//std::cerr << length << std::endl;

	Mafia::System::String str = Mafia::System::String((char*)strBytes, length);
	env->ReleaseByteArrayElements(data, strBytes, 0);

	try {

		int res = Mafia::System::Serializer::deserialize<int>(str);
		//std::cerr << str.data << std::endl;
		jbyte* strBytes1 = env->GetByteArrayElements(data, 0);
		for (int i = 0; i < str.size; i++)
		{
			strBytes1[i] = str.data[i];
		}
		strBytes1[str.size + 1] = 0;
		env->ReleaseByteArrayElements(data, strBytes1, 0);

		//std::cerr << "finished\n";
		return (jint)res;
	}
	catch (Mafia::Exceptions::Exception* ex) {
		raise_exception(env, ex);
		return 0;
	}
}

