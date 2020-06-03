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
        /*!
         * \brief На самом деле такой же конструктор, как и у других классов сообщения
         * (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))
         * \param data Описание ошибки
         * \param id id ошибки
         */
        RequestException(String data, ExceptionIdType id);
        /*!
         * \brief show Вывод сообщения
         */
        void show() override;

        void set_id(ExceptionIdType id) override;

        void set_data(String data) override;

        ExceptionIdType get_id() override;

        String get_data() override;

        static const ExceptionIdType defaultId = 300;
    };
}

//! Исключение, вызываемое при ответе на запрос не тем клиентом, которому этот запрос предназначался
#define WRONG_CLIENT_EXCEPTION_ID ((ExceptionIdType)1)
//! Исключение, вызываемое при несовпадении размеров типов данных - переданный для получения из ответа запроса и полученный в результате запроса
#define SIZE_NOT_MATCH_EXCEPTION_ID ((ExceptionIdType)2)
//! Исключение, вызываемое при попытке получить данные из запроса, который еще не завершил свою работу
#define NOT_FINISHED_EXCEPTION_ID ((ExceptionIdType)3)


#endif // REQUESTEXCEPTION_H
