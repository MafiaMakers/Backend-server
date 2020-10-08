#ifndef CLIENTINFO_H
#define CLIENTINFO_H
#include "Network/message.h"
#include "userstatistics.h"


namespace Mafia {
    //! \brief Структура для хранения и описания информации о клиенте, которая передается субсерверу-комнате
    struct ClientInfo{
		ClientInfo(QJsonObject src);
		ClientInfo();

        /*!
         * \brief Оператор сравнения
         * \param a Второй объект, с которым сравниваем
         * \return true, если эти два объекта описывают одного клиента, вошедшего под одним пользователем, иначе - false
         */
        bool operator == (ClientInfo a);

        /*!
         * \brief Обратный оператор сравнения
         * \param a Второй объект, с которым сравниваем
         * \return true, если эти объекты описывают не одного пользователя, иначе - false
         */
        bool operator != (ClientInfo a);

		QJsonObject to_json();

        //! \brief Клиент, которого описываем
        Network::Client client;

        //! \brief Информация о пользователе, под которым зашел этот клиент
        UserStatistics statistics;
    };
}

#endif // CLIENTINFO_H
