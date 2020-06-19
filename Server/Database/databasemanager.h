#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "databaseworker.h"

namespace Mafia {
    namespace Database {

        //! \brief Базовый класс, от которого наследуются все классы для работы с конкретными БД
        class DatabaseManager
        {
        protected:
            /*!
             * \brief Основной и единственный конструктор, который связывается с БД и конкретной таблицей
             * \param databaseWorker указатель на объект для работы с БД
             * \param name имя таблицы в этой БД
             */
            DatabaseManager(DatabaseWorker* databaseWorker, QString name);
            //! \brief Указатель на объект для работы с базой данных
            DatabaseWorker* dbWorker;
            //! \brief Локальный параметр хеширования
            const QString localParameter = "Par@!A#$^%&*()-_=+/\\MeTerLOCal";
            //! \brief Имя таблицы
            const QString dbName;

        };
    }
}



#endif // DATABASEMANAGER_H
