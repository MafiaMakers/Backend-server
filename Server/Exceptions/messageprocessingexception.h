#ifndef MESSAGEPROCESSINGEXCEPTION_H
#define MESSAGEPROCESSINGEXCEPTION_H
#include "messageparsingexception.h"

namespace Mafia {
    namespace Exceptions {
        /*! \brief Класс исключений, произошедших при обработке сообщения*/
        class MessageProcessingException : public MessageParsingException
        {
        public:
            //! \brief см. \ref Mafia::Exception::Exception()
            MessageProcessingException(){};
            /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
            MessageProcessingException(System::String data, ExceptionIdType id);
            //! \brief см. \ref Mafia::Exception::get_base_exception_id()
            ExceptionIdType get_base_exception_id() override;
        };

        //! \brief Возможные типы исключений обработки сообщения
        enum MessageProcessingExceptionId{
            //! \brief Базовый id исключения обработки сообщения
            MessageProcessingExceptionId_Default = 200,
            //! \brief Исключение, вызываемое при получении сообщения неизвестного типа
            MessageProcessingExceptionId_UnknownMessageType = 201,
            //! \brief Верхняя граница id исключений обработки сообщений (см. \ref Mafia::Database::Status_First)
            MessageProcessingExceptionId_Last = 202
        };
    }
}

#endif // MESSAGEPROCESSINGEXCEPTION_H
