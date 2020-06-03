#ifndef MESSAGEPROCESSINGEXCEPTION_H
#define MESSAGEPROCESSINGEXCEPTION_H
#include "messageparsingexception.h"

namespace Mafia {
    /*! \brief Класс исключений, произошедших при обработке сообщения*/
    class MessageProcessingException : public Mafia::MessageParsingException
    {
    public:
        /*! \brief Конструктор (см. \ref Mafia::MessageParsingException::MessageParsingException(String data, int id))*/
        MessageProcessingException(String data, int id);
        /*! \brief Значение id сообщения, с которого начинаются индексы сообщений этого класса*/
        static const ExceptionIdType defaultId = 200;
    };
}

//! Исключение, вызываемое при получении сообщения неизвестного типа
#define UNKNOWN_MESSAGE_TYPE_EXCEPTION_ID ((ExceptionIdType)1)

#endif // MESSAGEPROCESSINGEXCEPTION_H
