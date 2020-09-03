#ifndef JNIMETHODRUNNER_H
#define JNIMETHODRUNNER_H
#include "jni.h"
#include "Network/message.h"

namespace Mafia {
	namespace System {
		class JNIMethodRunner
		{
		public:
			static void setup(JNIEnv * env);

			static void run_messagesender_send(char* data, int size, int ip, int port);
			static void run_subservernetworker_processmessage(Mafia::Network::Message message);

			//static void run_static_method(const char* className, const char* methodName, const char* signature, ...);
		private:
			JNIMethodRunner();

			static JNIEnv * environment;
		};
	}
}


#endif // JNIMETHODRUNNER_H
