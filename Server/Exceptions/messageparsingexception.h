#ifndef MESSAGEPARSINGEXCEPTION_H
#define MESSAGEPARSINGEXCEPTION_H
#include "exception.h"



namespace Mafia {
    namespace Exceptions {
        //! \brief Класс для ошибок, связанных с распаковкой или запаковкой сообщений при отправке.
        class MessageParsingException : public Exception
        {
        public:
            /*!
             * \brief Пустой конструктор
             */
            MessageParsingException(){};
            /*! \brief см. \ref Mafia::Exception::Exception(Mafia::String data, ExceptionIdType id)*/
            MessageParsingException(System::String data, ExceptionIdType id);
            //! \brief см. \ref Mafia::Exception::show()
            void show() override;
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;
        };

        //! \brief Возможные тиаы исключений распаковки или запаковки сообщений
        enum MessageParsingExceptionId{
            //! \brief Базовый id исключения парсинга сообщения
            MessageParsingExceptionId_Default = 100,
            //! \brief Исключение, вызываемое при получении сообщения, слишком короткого для декодирования
            MessageParsingExceptionId_ShortMessage = 101,
            //! \brief Исключение, вызываемое при получении сообщения с невозможным id
            MessageParsingExceptionId_InvalidMessageId = 102,
            //! \brief Исключение, вызываемое при получении сообщения невозможного размера
            MessageParsingExceptionId_InvalidMessageSize = 103,
            //! \brief Исключение, вызываемое при получении сообщения с несовпадающей контрольной суммой
            MessageParsingExceptionId_ControlSumMismatch = 104,
            //! \brief Исключение, вызываемое при попытке дешифрования сообщения дешифратором с незаданным ключом
            MessageParsingExceptionId_NoneKey = 105,
            //! \brief Верхняя граница типов исключений парсинга сообщений (см. \ref Mafia::Database::Status_First)
            MessageParsingExceptionId_Last = 106
        };
    }
}


#endif // MESSAGEPARSINGEXCEPTION_H
