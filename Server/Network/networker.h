#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include "message.h"
#include "JSONMessage.h"
#include "tcpmessagesender.h"


typedef QJsonObject KeyValuePairSet;

namespace Mafia {
	namespace Network {
		class Networker : public QObject
		{
			Q_OBJECT
		public:
			//!\brief Qt-шный конструктор, который использовать не надо
			explicit Networker(QObject *parent = nullptr);

			/*!
			 * \brief Основной конструктор
			 * \param port Порт, на котором стоит инициализировать сервер
			 */
			Networker(int port);

			/*!
			 * \brief Функция отправки сообщения
			 * \param id Тип сообщения, которое отправляем
			 * \param data Список ключей и значений, которые стоит записать в сообщение
			 * (в нашем случае json)
			 * \param receiver Получатель сообщений
			 */
			void send_message(int id, KeyValuePairSet data, Client receiver);


		signals:
			/*!
			 * \brief Сигнал, вызываемый при получении сообщения
			 * \param message Полученное сообщение
			 */
			void message_received(JSONMessage message);
			/*!
			 * \brief Сигнал, вызываемый при получении сообщения от субсервера
			 * \param message Полученное сообщение
			 */
			void subserver_message_received(JSONMessage message);
			/*!
			 * \brief Сигнал, вызываемый при отключении клиента
			 * \param client Клиент, который был отключен
			 */
			void client_disconnected(Client client);


		private slots:
			/*!
			 * \brief Слот получения сообщения
			 * \param message Данные сообщения
			 * \param size Размер сообщения
			 * \param client Клиент, отправивший сообщение
			 */
			void on_message_received(char* message, int size, Client client);
			/*!
			 * \brief Слот отключения клиента
			 * \param client Клиент, который отключился
			 */
			void on_client_disconnected(Client client);

		private:
			/*!
			 * \brief Функция обработки полученного сообщения
			 * \param message Полученное сообщение
			 */
			void process_message(JSONMessage message);

			/*!
			 * \brief Функция перевода набора пар ключ-значение в json объект
			 * (так как сейчас json объект используется в качестве такого набора,
			 * функция просто возвращает переданный в нее параметр)
			 * \param kvpSet Набор пар ключ-значение
			 * \return json объек, который содержит в себе все пары ключ-значение
			 */
			QJsonObject KVPSet_to_json(KeyValuePairSet kvpSet);

			//!\brief Основной объект для отправки сообщений
			TCPMessageSender* mSender;
		};
	}
}

#endif // NETWORKER_H
