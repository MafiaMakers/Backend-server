#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include "System/simplestring.h"
#include <QtSql>

//! \brief Sql команда для обозначения переменной, которой нельзя присвоить неинициализированное значение
#define NOT_NULL " NOT NULL"
//! \brief Sql команда для обозначения основной переменной
#define UNIQUE " UNIQUE"

namespace Mafia {
    //! \brief Пространство имен, используемое для работы с базами данных
    namespace Database {
        /*!
         * \brief Класс, отвечающий за создание, поддержку резервное копирование и выполнение sql запросов базы данных
         */
        class DatabaseWorker
        {
        public:
            /*!
             * \brief Основной конструктор, инициализирующий параметры базы данных
             * \param _path Путь к папке базы данных (Стандартноое значение "")
             * \param _backupPath Путь к папке резервного копирования базы данных (Стандартное значение "D:\")
             * \param _filename Имя файла базы данных (Стандартное значение "MyDatabase.sqlite")
             */
            DatabaseWorker(System::String _path = System::String(""),
                           System::String _backupPath = System::String("D:\\"),
                           System::String _filename = System::String("MafiaDatabase.sqlite"));

            /*!
             * \brief Функция проверяющая готовность базы данных к работе с ней
             * \return true, если БД готова к работе, иначе false
             */
            bool database_ready();
            //! \brief Функция проверки открыта ли сейчас БД
            //! \return true, если БД открыта (не путайте с 'готова к работе'), иначе - false)
            bool database_open();

            /*!
             * \brief Функция, выполняющая строковой Sql запрос
             * \param request Запрос, который следует выполнить
             * \return Полученный результат в виде QSqlQuery*
             * \throws DatabaseWorkingExceptionId_SQlQuery в случае неудачного выполнения запроса
             */
            QSqlQuery* run_query(QString request);

            void restore_database();

        private:
            /*!
             * \brief Выполняет во втором потоке резервное копирование базы данных для возможности восстановления
             */
            void _database_backup();

            /*!
             * \brief Переменная, отвечающая за то, находится ли прямо сейчас база данных в процессе резервного копирования
             */
            bool isOnBackup;

            /*!
             * \brief Интерфейс базы данных в виде QSqlDatabase
             */
            QSqlDatabase dataBase;

            /*!
             * \brief Временной промежуток между двумя резервными копированиями (мс)
             */
            const int backupInterval = 10000;

            /*!
             * \brief Путь к папке базы данных
             */
            const System::String path;

            /*!
             * \brief Путь к папке резервной копии базы данных
             */
            const System::String backupPath;

            /*!
             * \brief Имя файла базы данных (в обеих файлах)
             */
            const System::String filename;

            /*!
             * \brief Максимальное время ожидания готовности базы данных.
             * Если это время превышается, будет выброшена ошибка о первышении времени ожидания
             */
            static const int maxWaitingTime = 2000;

        };

    }
}


#endif // DATABASEWORKER_H
