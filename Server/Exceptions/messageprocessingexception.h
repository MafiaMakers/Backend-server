#ifndef MESSAGEPROCESSINGEXCEPTION_H
#define MESSAGEPROCESSINGEXCEPTION_H
#include "messageparsingexception.h"

namespace Mafia {
    /*! \brief Класс исключений, произошедших при обработке сообщения*/
    class MessageProcessingException : public Mafia::MessageParsingException
    {
    public:
        //! \brief см. \ref Mafia::Exception::Exception()
        MessageProcessingException(){};
        /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
        MessageProcessingException(String data, ExceptionIdType id);
        //! \brief см. \ref Mafia::Exception::get_base_exception_id()
        ExceptionIdType get_base_exception_id() override;
    };
}

//! Базовый id исключения обработки сообщения
#define MESSAGE_PROCESSING_EXCEPTION_DEFAULT_EXCEPTION_ID 200

//! Исключение, вызываемое при получении сообщения неизвестного типа
#define UNKNOWN_MESSAGE_TYPE_EXCEPTION_ID (MESSAGE_PROCESSING_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)1)

#endif // MESSAGEPROCESSINGEXCEPTION_H
