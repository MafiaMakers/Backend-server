#ifndef SUBSERVEREXCEPTION_H
#define SUBSERVEREXCEPTION_H

#include "exception.h"

namespace Mafia {
    namespace Exceptions {
        /*!
         * \brief Класс ошибок работы с субсервером
         */
        class SubserverException : public Exception
        {
        public:
            //! \brief см. \ref Mafia::Exception::Exception()
            SubserverException(){}
            /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
            SubserverException(System::String data, ExceptionIdType id);
            //! \brief см. \ref Mafia::Exception::show()
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;
        };

        //! \brief Возможные id исключений работы с субсервером
        enum SubserverExceptionId{
            //! \brief Базовый id исключения субсервера
            SubserverExceptionId_Default = 400,
            //! \brief Исключение, вызываемое при попытке обратиться к результату запроса, которого не отправляли этому субсерверу
            SubserverExceptionId_NoSuchRequest = 401,
            //! \brief Верхняя граница id исключений работы с субсервером (см. \ref Mafia::Database::Status_First)
            SubserverExceptionId_Last = 402
        };
    }

}

#endif // SUBSERVEREXCEPTION_H
