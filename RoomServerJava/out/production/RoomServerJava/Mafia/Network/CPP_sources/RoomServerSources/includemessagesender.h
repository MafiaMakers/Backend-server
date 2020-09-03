#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

class MessageSender
{
public:
	static void send(char* data, int size, int ip, int port);

private:

	MessageSender(int port);
};

#define MESSAGE_SENDER MessageSender

#endif // MESSAGESENDER_H
