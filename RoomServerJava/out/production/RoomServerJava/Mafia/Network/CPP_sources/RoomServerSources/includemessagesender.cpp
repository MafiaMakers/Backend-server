#include <iostream>
#include "includemessagesender.h"
#include "jnimethodrunner.h"

MessageSender::MessageSender(int port)
{
	std::cout << "Initialized server at port: " << port << "\n";
}

void MessageSender::send(char* data, int size, int ip, int port)
{
	/*std::cout << "Sending message:" <<
				"\n	data: " << data <<
				"\n	size: " << size <<
				"\n	ip: "
							<< (int)(((char*)&ip)[3]) << "."
							<< (int)(((char*)&ip)[2]) << "."
							<< (int)(((char*)&ip)[1]) << "."
							<< (int)(((char*)&ip)[0]) <<
				"\n	port: " << port << "\n\n";*/
	Mafia::System::JNIMethodRunner::run_messagesender_send(data, size, ip, port);
}
