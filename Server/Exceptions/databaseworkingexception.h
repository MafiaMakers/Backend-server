#ifndef DATABASEWORKINGEXCEPTION_H
#define DATABASEWORKINGEXCEPTION_H

#include "exception.h"

namespace Mafia {
    namespace Exceptions {
         /*!
         * \brief Класс для описания исключений работы с БД
         */
        class DatabaseWorkingException : public Exception
        {
        public:
            /*!
             * \brief Пустой конструктор
             */
            DatabaseWorkingException();

            /*!
             * \brief см. \ref Mafia::Exceptions::Exception(System::String data, ExceptionIdType id)
             */
            DatabaseWorkingException(System::String data, ExceptionIdType id);

            /*!
             * \brief show Вывод сообщения
             */
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;
        };

        //! \brief Возможные id исключений, вызываемых работой с БД
        enum DatabaseWorkingExceptionId{
            //! \brief Базовое значение id исключений работы с БД (сообщений с таким id не бывает)
            DatabaseWorkingExceptionId_Default = 500,
            //! \brief Исключение, вызываемое при ошибке при резервном копировании БД
            DatabaseWorkingExceptionId_Backup = 501,
            //! \brief Исключение, вызываемое при ошибке в sql-запросе
            DatabaseWorkingExceptionId_SQlQuery =502,
            //! \brief Исключение, вызываемое при получении пустого ответа на sql-запрос
            DatabaseWorkingExceptionId_EmptyQueryResult = 503,
            //! \brief Исключение, вызываемое при попытке совершить действие, для которого необходима авторизация, без авторизации
            DatabaseWorkingExceptionId_NotAuthorizedAction = 504,
            //! \brief Исключение, вызываемое при несовпадении размеров данных, хранящихся в БД и ожидаемых
            DatabaseWorkingExceptionId_TypeMismatch = 505,
            //! \brief Исключение, вызываемое при превышении времени ожидания отклика БД
            DatabaseWorkingExceptionId_TimeLimit = 506,
            //! \brief Исключение, вызываемое при передаче неизвестного sql типа данных
            DatabaseWorkingExceptionId_UnknownType = 507,
            //! \brief Верхняя граница id исключений работы с БД (см. \ref Mafia::Database::Status_First)
            DatabaseWorkingExceptionId_Last = 508
        };
    }
}


#endif // DATABASEWORKINGEXCEPTION_H
