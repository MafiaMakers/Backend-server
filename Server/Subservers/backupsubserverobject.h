#ifndef BACKUPSUBSERVEROBJECT_H
#define BACKUPSUBSERVEROBJECT_H

#include "subserverobject.h"

namespace Mafia {
    namespace Subservers {
        //! \brief Класс для общения с субсервером, ответственным за восстановление основного сервера в случае краша
        class BackupSubserverObject : public SubserverObject
        {
        public:
            /*!
             * \brief Основной конструктор
             * \param networker Указатель на объект для работы с сетью (получение и отправление сообщений)
             * \param port Порт, на котором следует инициализировать субсервер
             * \param path Путь к испольняемому файлу субсервера
             * \param processName Имя процесса субсервера
             * \param checkInterval Временной промежуток, через который следует проверять отклик субсервера (мс) (стандартно равно 2000)
             * \param maxNotAnswering Максимальное количество пропущенных подрят запросов на отклик (стандартно равно 5)
             * \param specialCommands Дополнительные команды для передачи субсерверу
             */
            BackupSubserverObject(Network::MainServerNetworker* networker, int port,
                                  int checkInterval = 2000, int maxNotAnswering = 5, const System::String specialCommands = System::String(""));

        private slots:
            /*!
             * \brief Слот для получения сообщения от субсервера (автоматически привязывается к сигналу MainServerNetworker'а)
             * \param message Сообщение, которое получил сервер
             */
            void message_from_server(Network::Message message) override;

        private:
            /*!
             * \brief Метод постоянной проверки подключения
             */
            void check_connection();

            //! Путь к исполняемому файлу субсервера
            static const System::String exePath;

            //! Имя процесса субсервера
            static const System::String exeProcessName;
        };

    }
}

#endif // BACKUPSUBSERVEROBJECT_H
