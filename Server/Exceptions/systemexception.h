#ifndef SYSTEMEXCEPTION_H
#define SYSTEMEXCEPTION_H
#include "exception.h"

namespace Mafia {
    namespace Exceptions {
        /*!
         * \brief Класс исключений, связанных с выполнением системных функций
         */
        class SystemException : public Exception
        {
        public:
            /*!
             * \brief Пустой конструктор
             */
            SystemException();

            //! \brief см. \ref Mafia::Exception::show()
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;

			/*! \brief см. \ref Mafia::Exception::Exception(Mafia::String data, ExceptionIdType id)*/
			SystemException(System::String data, ExceptionIdType id);
        };

        /*!
         * \brief Перечисление типов исключений, связанных с выполнением системных функций
         */
        enum SystemExceptionId{
            //! \brief Базовый id таких исключений
            SystemExceptionId_Default = 800,
            //! \brief Исключение, вызываемое при получении для десериализации сообщения неверного размера,
            //! из которого невозможно извлечь необходимую структуру
            SystemExceptionId_InvalidMessageSize = 801,
            //! \brief Исключение, вызываемое при попытке выделить порт, когда все доступные порты уже заняты
            SystemExceptionId_PortAllocation = 802,
            //! \brief Исключение, вызываемое при попытке освободить порт, который не был выделен
            SystemExceptionId_PortFreeing = 803,
            //! \brief Исключение, вызываемое при ошибке выполнения сторонней программы (при вызове ее из командной строки)
            SystemExceptionId_ErrorInCmd = 804,
			//Ошибка при работе с записью логов
			SystemExceptionId_LogsError = 805,
            //! \brief Максимальный id таких исключений (см. \ref Mafia::Database::Status_Last)
			SystemExceptionId_Last = 806,
        };
    }
}


#endif // SYSTEMEXCEPTION_H
