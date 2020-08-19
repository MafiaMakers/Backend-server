#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "System/simplestring.h"

/*! \brief Тип, который применяется для указания id сообщения. Пока что там int. Позже, возможно, будет что-то другое.*/
typedef int ExceptionIdType;

namespace Mafia {
    //! Пространство имен для описания и работы с исключениями
    namespace Exceptions {
        /*! \brief Базовый класс для ошибок. Его экземпляры создавать нельзя, но все классы ошибок должны наследоваться от него.
         * Соответственно в try-catch блоке стоит использовать Exception* вместо конкретных дочерних классов.*/
        class Exception
        {
        public:
            /*! \brief Пустой конструктор для переопределения его в дочерних классах.*/
            Exception(){}

			virtual ~Exception();

            /*! \brief Функция для вывода сообщения об ошибке в случае, если ошибка все же не обработана.*/
            virtual void show();
            /*! \brief Задавание параметра data
            * \param data - То, что будет использовано в качестве данных
            */
            virtual void set_data(System::String data);
            /*! \brief Задавание параметра id
            * \param id - То, что будет использовано в качестве id ошибки
            */
            virtual void set_id(ExceptionIdType id);
            /*! \brief Взятие параметра id
            * \returns id данного исключения
            */
            virtual ExceptionIdType get_id();
            /*! \brief Взятие параметра data
            * \returns сообщение данного исключения
            */
            virtual System::String get_data();

            /*!
             * \brief Функция получения базового id исключения, с которого начинается отсчет id всех исключений этого типа
             * \return базовый id исключения (исключений с таким id не бывает)
             */
			virtual ExceptionIdType get_base_exception_id();

			static Exception* generate(System::String data, ExceptionIdType id);

			virtual void close();
        protected:
			/*!
			 * \brief Основной конструктор, который инициализирует параметры исключения
			 * \param data см. \ref Mafia::Excepton::data
			 * \param id см. \ref Mafia::Exception::data
			 */
			Exception(System::String data, ExceptionIdType id);

            //! \brief Сообщение, которое может быть использовано для отображения сообщения об ошибке
            System::String data;
            //! \brief id исключения для отличия одних от других
            ExceptionIdType id;
        };
    }

}


#endif // EXCEPTION_H
