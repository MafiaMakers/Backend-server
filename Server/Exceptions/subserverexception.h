#ifndef SUBSERVEREXCEPTION_H
#define SUBSERVEREXCEPTION_H

#include "exception.h"

namespace Mafia {

    class SubserverException : public Mafia::Exception
    {
    public:
        SubserverException(){}

        SubserverException(String data, ExceptionIdType id);

        void show() override;

        ExceptionIdType get_base_exception_id() override;
    };
}

#define SUBSERVER_EXCEPTION_DEFAULT_EXCEPTION_ID 400
#define NO_SUCH_REQUEST_EXCEPTION_ID (SUBSERVER_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)1)

#endif // SUBSERVEREXCEPTION_H
