#ifndef MAINSERVERMANAGER_H
#define MAINSERVERMANAGER_H
#include "Network/mainservernetworker.h"
#include "Exceptions/exception.h"
#include "Requests/networkrequest.h"
#include <thread>
#include <chrono>

namespace Mafia {
    /*!
     * \brief Главный управляющий класс основного сервера. Он пока еще не дописан. Только начало...
     */
    class MainServerManager : public QObject
    {
        Q_OBJECT
    public:
        /*!
         * \brief Конструктор, который также создает \ref MainServerNetworker и пока что для теста еще создает 1 тестовый запрос
         */
        explicit MainServerManager(QObject * parent = nullptr);

    private:
        /*!
         * \brief Получает данные от уже инициализированного сетевого запроса
         * \param req Указатель на запрос
         */
        void _get_data_from_request(Requests::NetworkRequest* req);
        //! Указатель на объект сетевого класса
        MainServerNetworker* networker;

    };
}

#endif // MAINSERVERMANAGER_H
