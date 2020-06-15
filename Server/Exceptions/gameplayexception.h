#ifndef GAMEPLAYEXCEPTION_H
#define GAMEPLAYEXCEPTION_H
#include "exception.h"

namespace Mafia {
    namespace Exceptions {
         /*!
         * \brief Класс для описания исключений, связанных с ролями, игрой и процессом игры
         */
        class GameplayException : public Exception
        {
        public:
            //! \brief Пустой конструктор
            GameplayException();
            /*! \brief см. \ref Mafia::Exception::Exception(Mafia::String data, ExceptionIdType id)*/
            GameplayException(System::String data, ExceptionIdType id);
            //! \brief см. \ref Mafia::Exception::show()
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;
        };

        //! \brief Возможные id иключений геймплея
        enum GamePlayExceptionId{
            //! \brief Базовое значение исключений этого типа
            GameplayExceptionId_Default = 600,
            //! \brief Исключение, вызываемое при получении неизвестного id роли
            GameplayExceptionId_UnknownRole = 601,
            //! \brief Верхняя граница индексов исключений геймплея (см. \ref Mafia::Database::Status_First)
            GameplayExceptionId_Last = 602
        };
    }
}



#endif // GAMEPLAYEXCEPTION_H
