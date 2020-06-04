#ifndef MESSAGEPARSINGEXCEPTION_H
#define MESSAGEPARSINGEXCEPTION_H
#include "exception.h"



namespace Mafia {
    //! \brief Класс для ошибок, связанных с распаковкой или запаковкой сообщений при отправке.
    class MessageParsingException : public Mafia::Exception
    {
    public:
        MessageParsingException(){};
        /*! \brief Конструктор, который просто заполняет поля класса
         * \param data соответствует полю data (см. \ref Mafia::Exception)
         * \param id соответствует полю id (см. \ref Mafia::Exception)
         */
        MessageParsingException(String data, int id);
        //! \brief см. \ref Mafia::Exception
        void show() override;

        ExceptionIdType get_base_exception_id() override;
    };
}

#define MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID 100

//! Исключение, вызываемое при получении сообщения, слишком короткого для декодирования
#define SHORT_MESSAGE_EXCEPTION_ID (MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)1)
//! Исключение, вызываемое при получении сообщения с невозможным id
#define INVALID_MESSAGE_ID_EXCEPTION_ID (MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)2)
//! Исключение, вызываемое при получении сообщения невозможного размера
#define INVALID_MESSAGE_SIZE_EXCEPTION_ID (MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)3)
//! Исключение, вызываемое при получении сообщения с несовпадающей контрольной суммой
#define CONTROL_SUM_EXCEPTION_ID (MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)4)
//! Исключение, вызываемое при попытке дешифрования сообщения дешифратором с незаданным ключом
#define NONE_KEY_EXCEPTION_ID (MESSAGE_PARSING_EXCEPTION_DEFAULT_EXCEPTION_ID + (ExceptionIdType)5)

#endif // MESSAGEPARSINGEXCEPTION_H
