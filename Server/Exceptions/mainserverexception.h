#ifndef MAINSERVEREXCEPTION_H
#define MAINSERVEREXCEPTION_H
#include "exception.h"
namespace Mafia {
    namespace Exceptions {
        //! \brief Класс для исключений, вызванных работой главного менеджера основного сервера
        class MainServerException : public Exception
        {
        public:
            /*!
             * \brief Пустой конструктор
             */
            MainServerException();

            //! \brief см. \ref Mafia::Exception::show()
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;

			/*!
			 * \brief Основной конструктор, наследуемый от \ref Mafia::Exceptions::Exception(System::String data, ExceptionIdType id)
			 * \param data Данные сообщения
			 * \param id id сообщения
			 */
			MainServerException(System::String data, ExceptionIdType id);
        };

        //! \brief Перечисление типов исключений, связанных с работой основного контроллера сервера
        enum MainServerExceptionId{
            //! \brief Основной id исключений такого типа
            MainServerExceptionId_Default = 700,
            //! \brief Исключение, вызываемое при попытке получить клиента, который авторизован под данным пользователем,
            //! когда никто не авторизован под данным пользователем
            MainServerExceptionId_NoSuchUser = 701,
            //! \brief Исключение, вызываемое при попытке получить пользователя, под которым авторизован данный клиент,
            //! когда данный клиент не авторизован ни под каким пользователем (или не онлайн сейчас)
            MainServerExceptionId_NoSuchClient = 702,
            //! \brief Исключение, вызываемое при получении транзакции неизвестного или неверного типа
            MainServerExceptionId_UnknownTransactionType = 703,
            //! \brief Исключение, вызываемое при попытке работы с игрой, которая не запущена/завершена/не существовала
            MainServerExceptionId_NoSuchGame = 704,
			//! \brief Исключение, вызываемое при обращении неавторизованного пользователя
			MainServerExceptionId_NullUserRequest = 705,
            //! \brief Максимальное id исключения (см. \ref Mafia::Database::Status_Last)
			MainServerExceptionId_Last = 706
        };
    }
}


#endif // MAINSERVEREXCEPTION_H
