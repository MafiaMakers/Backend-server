#ifndef SUBSERVEREXCEPTION_H
#define SUBSERVEREXCEPTION_H

#include "exception.h"

namespace Mafia {
    /*!
     * \brief Класс ошибок работы с субсервером
     */
    class SubserverException : public Mafia::Exception
    {
    public:
        //! \brief см. \ref Mafia::Exception::Exception()
        SubserverException(){}
        /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
        SubserverException(String data, ExceptionIdType id);
        //! \brief см. \ref Mafia::Exception::show()
        void show() override;
        //! \brief см. \ref Mafia::Exception::get_base_exception_id()
        ExceptionIdType get_base_exception_id() override;
    };
}

//! Базовый id исключения субсервера
#define SUBSERVER_EXCEPTION_DEFAULT_EXCEPTION_ID 400
//! Исключение, вызываемое при попытке обратиться к результату запроса, которого не отправляли этому субсерверу
#define NO_SUCH_REQUEST_EXCEPTION_ID (SUBSERVER_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)1)

#endif // SUBSERVEREXCEPTION_H
