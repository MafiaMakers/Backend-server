#include "Mafia_Network_MessageParser.h"
#include "Network/crypto.h"
#include "MafiaExceptions"

char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void raise_exception(JNIEnv* env, Mafia::Exceptions::Exception* exception) {
	jclass excClass = env->FindClass("Mafia/Exceptions/MafiaException");
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID strGen = env->GetStaticMethodID(strClass, "valueOf", "([C)Ljava/lang/String;");
	jmethodID excGen = env->GetStaticMethodID(excClass, "generate", "(LMafia/Exceptions/ExceptionId;Ljava/lang/String;)LMafia/Exceptions/MafiaException;");
	jcharArray arr = env->NewCharArray(exception->get_data().size + 9);
	jchar* chars = env->GetCharArrayElements(arr, 0);

	for (int i = 0; i < exception->get_data().size; i++)
	{
		chars[i] = exception->get_data().data[i];
	}
	chars[exception->get_data().size] = ':';

	for (int i = 0; i < 4; i++)
	{
		chars[exception->get_data().size + 2 * i + 1] = hex[((unsigned int)((exception->get_id() >> (3 - i))) % 256) / 16];
		chars[exception->get_data().size + 2 * i + 2] = hex[((unsigned int)((exception->get_id() >> (3 - i))) % 256) % 16];
	}

	env->ReleaseCharArrayElements(arr, chars, 0);
	jobject strEx = env->CallStaticObjectMethod(strClass, strGen, arr);
	jclass exIds = env->FindClass("Mafia/Exceptions/ExceptionId");
	jfieldID fid = env->GetStaticFieldID(exIds, "NativeException", "LMafia/Exceptions/ExceptionId;");

	jobject fval = env->GetStaticObjectField(exIds, fid);

	jobject EXCEPTION = env->CallStaticObjectMethod(excClass, excGen, fval, strEx);

	env->Throw((jthrowable)EXCEPTION);
}

jobject JNICALL Java_Mafia_Network_MessageParser_parse_1message
(JNIEnv* env, jclass cls, jbyteArray arr, jint size, jint ip, jint port) {
	try {
		jbyte* values = env->GetByteArrayElements(arr, 0);

		char* data = new char[size];
		for (int i = 0; i < size; i++)
		{
			data[i] = (char)values[i];
		}


		Mafia::Network::Message* message = new Mafia::Network::Message();
		*message = Mafia::Network::Crypto::parse_data(data, size);

		message->client.ip = ip;
		message->client.port = port;

		jclass messageClass = env->FindClass("Mafia/Network/MessageJ");

		jmethodID constructor = env->GetMethodID(messageClass, "<init>", "(J)V");

		jobject messageObject = env->NewObject(messageClass, constructor, (jlong)(__int64)message);

		return messageObject;
	}
	catch (Mafia::Exceptions::Exception* exception) {
		raise_exception(env, exception);
		return 0;
	}
}

jbyteArray JNICALL Java_Mafia_Network_MessageParser_wrap_1message
(JNIEnv* env, jclass cls, jobject messageObject) {
	try {
		jclass messageClass = env->GetObjectClass(messageObject);
		jfieldID fid = env->GetFieldID(messageClass, "_pointer", "J");

		jlong ptr = env->GetLongField(messageObject, fid);
		Mafia::Network::Message* message = (Mafia::Network::Message*)(__int64)ptr;

		Mafia::System::String wrapped = Mafia::Network::Crypto::wrap_message(*message);

		jbyteArray result = env->NewByteArray(wrapped.size);

		jbyte* bytes = env->GetByteArrayElements(result, 0);
		for (int i = 0; i < wrapped.size; i++)
		{
			bytes[i] = (jbyte)wrapped.data[i];
		}

		env->ReleaseByteArrayElements(result, bytes, 0);

		return result;
	} catch (Mafia::Exceptions::Exception* exception) {
		raise_exception(env, exception);
		return 0;
	}
}

void JNICALL Java_Mafia_Network_MessageParser_init
(JNIEnv* env, jclass cls) {
	Mafia::Network::Crypto::set_key("DICH_KEY");
}