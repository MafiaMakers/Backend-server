#ifndef PROCESSOROBJECT_H
#define PROCESSOROBJECT_H
#include "Network/messageprocessor.h"
#include "Exceptions/messageparsingexception.h"
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
				static ProcessorObject* generate(Message_t message);

				template<class T>
				/*!
				 * \brief Получает типизированное значение из json
				 * \param obj Объект json, в котором следует искать значение
				 * \param key Ключ, по которому стоит искать
				 * \return Значение по ключу из json
				 */
				static T get_from_json(QJsonObject obj, QString key);

				virtual ~ProcessorObject();

                /*!
                 * \brief Метод обработки сообщений, который в базовом классе чисто виртуальный, но определен в каждом наследнике
                 */
                virtual void process() = 0;

            protected:
                /*!
                 * \brief Основной конструктор, который будет использоваться во всех наследниках (там он будет публичным)
                 * \param message Сообщение, которое должен обработать обработчик
                 */
				ProcessorObject(Message_t message);

				/*!
				 * \brief Используется в \ref get_from_json. Проверяет наличие id в json, если все ок, то возвращает значение,
				 * если что-то не так, то кидает исключение
				 * \param obj Объект json, в котором следует искать значение
				 * \param key Ключ, по которому стоит искать
				 * \return значение (без типа), полученное из json
				 */
				static QJsonValue get_json_value(QJsonObject obj, QString key);

                //! \brief Отправитель сообщения, которое следует обработать
                Client sender;
                //! \brief Данные сообщения
				QJsonObject data;
                //! \brief id сообщения
				MessageIdType id;
            };
			/*! \brief Получение из json переменной. Создает переменную с именем name типа T и получает из объекта obj ее значение по ключу name
			 *  \param T тип переменной, которую следует создать и получить из json
			 *  \param name имя переменной, которую следует создать и ключ, по которому следует получать значение из json
			 *  \param obj QJsonObject, из которого следует получать значение переменной
			 */
			#define GET_FROM_JSON(T, name, obj) T name = ProcessorObject::get_from_json<T>(obj, #name);

			/*! \brief Аналогично \ref GET_FROM_JSON, но производит явное приведение типов:
			 * создает объект name типа T, получает из json по ключу name объект типа vT и приводит его явно к T
			 * Удобно использовать для enum'ов, которые хранятся как int.
			 */
			#define GET_FROM_JSON_T(T, name, vT, obj) T name = (T)ProcessorObject::get_from_json<vT>(obj, #name);
        }
    }
}


#endif // PROCESSOROBJECT_H
