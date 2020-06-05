#ifndef REQUESTEXCEPTION_H
#define REQUESTEXCEPTION_H
#include "exception.h"

namespace Mafia {
    /*!
     * \brief Класс исключений, связанных с работой системы запросов
     */
    class RequestException : public Mafia::Exception
    {
    public:
        //! \brief см. \ref Mafia::Exception::Exception()
        RequestException(){}
        /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
        RequestException(String data, ExceptionIdType id);
        /*!
         * \brief show Вывод сообщения
         */
        void show() override;
        //! \brief см. \ref Mafia::Exception::get_base_exception_id()
        ExceptionIdType get_base_exception_id() override;

    };
}

//! Базовый id иключения, связанного с запросами
#define REQUEST_EXCEPTION_DEFAULT_EXCEPTION_ID 300

//! Исключение, вызываемое при ответе на запрос не тем клиентом, которому этот запрос предназначался
#define WRONG_CLIENT_EXCEPTION_ID (REQUEST_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)1)
//! Исключение, вызываемое при несовпадении размеров типов данных - переданный для получения из ответа запроса и полученный в результате запроса
#define SIZE_NOT_MATCH_EXCEPTION_ID (REQUEST_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)2)
//! Исключение, вызываемое при попытке получить данные из запроса, который еще не завершил свою работу
#define NOT_FINISHED_EXCEPTION_ID (REQUEST_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)3)


#endif // REQUESTEXCEPTION_H
