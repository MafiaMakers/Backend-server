#ifndef MESSAGEPARSINGEXCEPTION_H
#define MESSAGEPARSINGEXCEPTION_H
#include "Exceptions/exception.h"



namespace Mafia {
    //! \brief Класс для ошибок, связанных с распаковкой или запаковкой сообщений при отправке.
    class MessageParsingException : public Mafia::Exception
    {
    public:
        /*! \brief Конструктор, который просто заполняет поля класса
         * \param data соответствует полю data (см. \ref Mafia::Exception)
         * \param id соответствует полю id (см. \ref Mafia::Exception)
         */
        MessageParsingException(String data, int id);
        //! \brief см. \ref Mafia::Exception
        void show() override;
        //! \brief см. \ref Mafia::Exception
        void set_data(String data) override;
        //! \brief см. \ref Mafia::Exception
        void set_id(ExceptionIdType id) override;
        //! \brief см. \ref Mafia::Exception
        ExceptionIdType get_id() override;
        //! \brief см. \ref Mafia::Exception
        String get_data() override;
        //! \brief id, с которого начинается отсчет всех id исключений этого типа
        static const ExceptionIdType defaultId = 100;
    };
}

#define SHORT_MESSAGE_EXCEPTION_ID (MessageParsingException::defaultId + 1)
#define INVALID_MESSAGE_ID_EXCEPTION_ID (MessageParsingException::defaultId + 2)
#define INVALID_MESSAGE_SIZE_EXCEPTION_ID (MessageParsingException::defaultId + 3)
#define CONTROL_SUM_EXCEPTION_ID (MessageParsingException::defaultId + 4)

#endif // MESSAGEPARSINGEXCEPTION_H
