#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H
#include "request.h"
#include "mainservernetworker.h"
#include "Exceptions/requestexception.h"
namespace Mafia {
    /*!
     * \brief Класс сетевого запроса. Получает сообщение, отправляет его нужному клиенту
     * и по получении ответа готов выдать результат в виде нужного типа данных.
     * Таким образом он бездействует до получения данных от клиента и взять данные в этот момент нельзя
     */
    class NetworkRequest : public Mafia::Request
    {
        Q_OBJECT
    public:
        /*!
         * \brief Единственный конструктор. Создает сетевой запрос и сразу отправляет сообщение с ним
         * \param _networker Указатель на объект работы с сетью
         * \param toAsk Сообщение запроса
         */
        NetworkRequest(MainServerNetworker* _networker, Message toAsk);

        /*!
         *  \brief Метод получения данных. Работает только когда запрос уже закончил свою работу, иначе кидает ошибку. Возвращает полученные данные
         */
        template<class T>
        T get_result()
        {
            if(this->finished){
                if(this->size == sizeof(T)){
                    return(*(T*)(this->data));
                } else{
                    throw new RequestException(String("Size of message doesn't match to size of given type"), SIZE_NOT_MATCH_EXCEPTION_ID);
                }
            } else{
                throw new RequestException(String("Request hasn't finished yet!"), NOT_FINISHED_EXCEPTION_ID);
            }

        }

        /*!
         * \brief Метод получения данных. Сначала ждет, когда данные будут готовы к получению и только после этого возвращает значение.
         * Будьте аккуратны с этим методом, так как он ждет в том же потоке, откуда был вызван
         */
        template<class T>
        T get_when_finished()
        {
            sleep_untill_finished();
            return get_result<T>();
        }
    private slots:
        /*!
         * \brief Слот для проверки, получены ли данные этого запроса. Слот привязан к серверному сигналу получения ответа на запрос
         * \param received Полученное сообщение
         */
        void tryClose(Message received);
    private:
        //! id сообщения, которое было отправлено изначально
        const MessageIdType id;
        //! Клиент, которому было отправлено сообщение
        const Client client;
        //! Полученные данные
        SymbolType* data;
        //! Размер массива полученных данных
        int size;
    };
}
#endif // NETWORKREQUEST_H
