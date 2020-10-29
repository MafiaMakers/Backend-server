#ifndef TCPMESSAGESENDER_H
#define TCPMESSAGESENDER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "message.h"

namespace Mafia {
	namespace Network {
		class TCPMessageSender : public QObject
		{
			Q_OBJECT
		public:
			explicit TCPMessageSender(QObject *parent = nullptr);

			/*!
			 * \brief Основной конструктор
			 * \param port Порт, на котором следует инициализировать сервер
			 */
			TCPMessageSender(int port);

			/*!
			 * \brief Основная функция отправки сообщения клиенту
			 * \param data Данные, которые следует отправить
			 * \param size Количество данных (в байтах)
			 * \param client Клиент, которому следует отправить данные
			 */
			void send(char* data, int size, Client client);

		signals:
			/*!
			 * \brief Сигнал, который отправляется при получении сообщения от клиента
			 * \param data Данные, которые получены
			 * \param size Количество байт данных
			 * \param client Клиент, который их отправил
			 */
			void message_received(char* data, int size, Client client);

			/*!
			 * \brief Сигнал, который вызывается при отключении клиента
			 * \param client Клиент, который отключился
			 */
			void on_client_disconnected(Client client);

		private:
			//!\brief Основной сервер-сокет. Принимает входящие подключения
			QTcpServer* mainSocket;
			//!\brief Множество клиентов, которые можно получить по ip и порту, которые у них есть
			QMap<Client,QTcpSocket *> clientSockets;

		private slots:
			/*!
			 * \brief Слот, вызываемый при подключении нового клиента
			 */
			void add_user();

			/*!
			 * \brief Слот, вызываемый при получении нового сообщения
			 */
			void receive_message();

			/*!
			 * \brief Слот, вызываемый при отключении пользователя
			 */
			void disconnect_user();

		};
	}
}

#endif // TCPMESSAGESENDER_H
