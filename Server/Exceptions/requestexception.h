#ifndef REQUESTEXCEPTION_H
#define REQUESTEXCEPTION_H
#include "exception.h"

namespace Mafia {
    namespace Exceptions {
        /*!
         * \brief Класс исключений, связанных с работой системы запросов
         */
        class RequestException : public Exception
        {
        public:
            //! \brief см. \ref Mafia::Exception::Exception()
            RequestException(){}
            /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
            RequestException(System::String data, ExceptionIdType id);
            /*!
             * \brief show Вывод сообщения
             */
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;

        };

        //! \brief Возможные id исключений работы системы запросов
        enum RequestExceptionId{
            //! \brief Базовый id иключения, связанного с запросами
            RequestExceptionId_Defaut = 300,
            //! \brief Исключение, вызываемое при ответе на запрос не тем клиентом, которому этот запрос предназначался
            RequestExceptionId_WrongClient = 301,
            //! \brief Исключение, вызываемое при несовпадении размеров типов данных - переданный для получения из ответа запроса и полученный в результате запроса
            RequestExceptionId_SizeMismatch = 302,
            //! \brief Исключение, вызываемое при попытке получить данные из запроса, который еще не завершил свою работу
            RequestExceptionId_NotFinished = 303,
            //! \brief Верхняя граница id исключений работы с запросами (см. \ref Mafia::Database::Status_First)
            RequestExceptionId_Last = 304
        };
    }
}

#endif // REQUESTEXCEPTION_H
