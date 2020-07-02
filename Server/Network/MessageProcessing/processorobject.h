#ifndef PROCESSOROBJECT_H
#define PROCESSOROBJECT_H
#include "Network/messageprocessor.h"
#include "Network/message.h"
#include "Network/messageTypes.h"


namespace Mafia {
    namespace Network {
        //! \brief Пространство имен для обработки входящих сообщений. Здесь есть лишь классы-обработчики сообщений
        namespace MessageProcessing {
            //! \brief Базовый класс всех обработчиков сообщений (нельзя создавать напрямую его экземпляр)
            class ProcessorObject
            {
            public:
                /*!
                 * \brief По сути, фабричный метод, который создает разные обработчики сообщений (наследники от этого класса).
                 * \param message
                 * \return Указатель на объект-обработчик, который с учетом типа сообщения обработает сообщение при вызове метода
                 * \ref Mafia::Network::MessageProcessing::ProcessorObject::process()
                 */
                static ProcessorObject* generate(Message message);

                /*!
                 * \brief Метод обработки сообщений, который в базовом классе чисто виртуальный, но определен в каждом наследнике
                 */
                virtual void process() = 0;

            protected:
                /*!
                 * \brief Основной конструктор, который будет использоваться во всех наследниках (там он будет публичным)
                 * \param message Сообщение, которое должен обработать обработчик
                 */
                ProcessorObject(Message message);

                //! \brief Отправитель сообщения, которое следует обработать
                Client sender;
                //! \brief Данные сообщения
                System::String data;
                //! \brief id сообщения
                MessageIdType id;
            };
        }
    }
}


#endif // PROCESSOROBJECT_H
