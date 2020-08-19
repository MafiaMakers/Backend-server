#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H


class MessageSender
{
public:
	MessageSender(int port);

	void send(char* data, int size, int ip, int port);
};

#define MESSAGE_SENDER MessageSender

#endif // MESSAGESENDER_H
