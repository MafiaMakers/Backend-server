#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H
#include <QUdpSocket>

namespace Mafia {
	namespace Network {
		class MessageSender : public QObject
		{
		Q_OBJECT
		public:
			MessageSender(int port);

			void send(char* message, unsigned int size, QString ip, int port);
			void send(char* message, unsigned int size, int ip, int port);

		signals:
			void message_received(char* data, int size, int ip, int port);

		private slots:
			void receive_message();

		private:
			QUdpSocket* socket;
			const int myPort;
		};
	}
}

#endif // MESSAGESENDER_H
