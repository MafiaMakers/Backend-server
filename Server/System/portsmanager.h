#ifndef PORTSMANAGER_H
#define PORTSMANAGER_H
#include "functions.h"

namespace Mafia {
    namespace System {
        //! \brief Класс для менеджмента свободных и занятых интернет-портов
        class PortsManager
        {
        public:
            /*!
             * \brief Функция, проверяющая, свободен ли сейчас данный порт
             * \param port Порт, который следует проверить
             * \return true, если порт свободен, иначе - false
             */
            static bool is_port_free(int port);

            /*!
             * \brief Функция выделения свободного порта
             * \return Выделенный порт
             */
            static int allocate_port();

            /*!
             * \brief Функция освобождения порта
             * \param port Порт, который следует освободить
             */
            static void free_port(int port);

        private:
            //! \brief Индекс порта, ниже которого не выделяем порты
            static const int MIN_PORT = 10000;
            //! \brief Индекс порта, выше которого не выделяем порты
            static const int MAX_PORT = 20000;
            //! \brief Список всех портов, которые сейчас заняты
            static MafiaList<int> allocatedPorts;
        };
    }
}


#endif // PORTSMANAGER_H
