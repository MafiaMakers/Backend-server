#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "databaseworker.h"

namespace Mafia {
    namespace Database {
        //! \brief Тип фильтра (используется для выбора нескольких объектов из БД).
        //! Например, можно выбирать все игры, в которых были все три игрока (\ref Mafia::Database::FilterType_AND),
        //! или все игры, в которых был хоть кто-то из троих (\ref Mafia::Database::FilterType_OR)
        enum FilterType{
            //! \brief Отсутствие фильтра
            FilterType_NONE,
            //! \brief Тип ИЛИ
            FilterType_OR,
            //! \brief Тип И
            FilterType_AND
        };


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
