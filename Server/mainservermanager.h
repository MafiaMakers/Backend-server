#ifndef MAINSERVERMANAGER_H
#define MAINSERVERMANAGER_H
#include "mainservernetworker.h"
#include "Exceptions/exception.h"
#include "networkrequest.h"
#include <thread>
#include <chrono>

namespace Mafia {
    /*!
     * \brief Главный управляющий класс основного сервера. Он пока еще не дописан. Только начало...
     */
    class MainServerManager
    {
    public:
        /*!
         * \brief Конструктор, который также создает \ref MainServerNetworker и пока что для теста еще создает 1 тестовый запрос
         */
        MainServerManager();
    private:
        /*!
         * \brief Получает данные от уже инициализированного сетевого запроса
         * \param req Указатель на запрос
         */
        void _get_data_from_request(NetworkRequest* req);
        //! Указатель на объект сетевого класса
        MainServerNetworker* networker;

    };
}

#endif // MAINSERVERMANAGER_H
