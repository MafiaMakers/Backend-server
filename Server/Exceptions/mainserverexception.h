#ifndef MAINSERVEREXCEPTION_H
#define MAINSERVEREXCEPTION_H
#include "exception.h"
namespace Mafia {
    namespace Exceptions {
        class MainServerException : public Exception
        {
        public:
            MainServerException();

            MainServerException(System::String data, ExceptionIdType id);

            //! \brief см. \ref Mafia::Exception::show()
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;
        };

        enum MainServerExceptionId{
            MainServerExceptionId_Default = 700,
            MainServerExceptionId_NoSuchUser = 701,
            MainServerExceptionId_NoSuchClient = 702,
            MainServerExceptionId_Last = 703
        };
    }
}


#endif // MAINSERVEREXCEPTION_H
