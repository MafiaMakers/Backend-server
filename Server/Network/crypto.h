#ifndef CRYPTO_H
#define CRYPTO_H
#include "message.h"
#include <iostream>
#include "System/simplestring.h"
#include "System/limitedqueue.h"
namespace Mafia {
    /*! \brief По сути статический класс
    * (кажется, в c++ нельзя делать статические классы, так что я просто сделал класс со всеми полями и методами статическими)
    * Класс используется для кодирования, декодирования, распаковки и запаковки сообщений перед отправкой или после получения по сети.
    */
    class Crypto
    {
    public:
        //! \brief Метод задавания ключа шифрования
        static void setKey(String key);
        //! \brief см. \ref Mafia::Crypto::setKey(String key)
        static void setKey(char* key, int size);
        //! \brief см. \ref Mafia::Crypto::setKey(String key)
        static void setKey(std::string key);
        /*! \brief Метод для распаковки сообщения
         * \param data массив символов - полученное по сети сообщение
         * \param size размер сообщения в байтах
         * \returns \ref Mafia::Message - сообщение, в котором описаны все параметры, КРОМЕ отправителя
         */
        static Message parse_data(char* data, int size);
        //! см. \ref Mafia::Crypto::parse_data(char* data, int size)
        static Message parse_data(String data);
        /*!
         * \brief Оборачивает сообщение в необходимую оболочку и подготавливает его к отправке по сети
         * \param mes \ref Mafia::Message сообщение, которое нужно подготовить к отправке
         * \return \ref Mafia::String строку, которую надо отправить по сети
         */
        static String wrap_message(Message mes);
    private:
        /*!
         * \brief Функция для проверки того, не противоречит ли чему-либо полученный message id
         * \param id id, который необходимо проверить
         * \return true, если все хорошо, false - если есть противоречие
         */
        static bool _message_id_ok(MessageIdType id);
        /*!
         * \brief Декодирует сообщение, полученное по сети
         * \param закодированное сообщение
         * \return декодированное сообщение
         */
        static String _decrypt(String encrypted);
        /*!
         * \brief Кодирует незакодированное сообщение
         * \param decrypted незакодированное сообщение
         * \return закодированное сообщение
         */
        static String _encrypt(String decrypted);
        //! Ключ шифрования
        static String key;
        //! Последние несколько id сообщений
        static LimitedQueue<MessageIdType> *lastMessageIds;
        //! Количество id сообщений, которые необходимо удерживать в памяти
        static const int rememberMessagesCount = 2;
    };
}

#endif // CRYPTO_H
