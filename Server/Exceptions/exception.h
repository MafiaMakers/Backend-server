#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "System/simplestring.h"

/*! \brief Тип, который применяется для указания id сообщения. Пока что там int. Позже, возможно, будет что-то другое.*/
typedef int ExceptionIdType;

namespace Mafia {
    /*! \brief Базовый класс для ошибок. Его экземпляры создавать нельзя, но все классы ошибок должны наследоваться от него.
     * Соответственно в try-catch блоке стоит использовать Exception* вместо конкретных дочерних классов.*/
    class Exception
    {
    public:
        /*! \brief Пустой конструктор для переопределения его в дочерних классах.*/
        Exception(){}

        Exception(String data, ExceptionIdType id);
        /*! \brief Функция для вывода сообщения об ошибке в случае, если ошибка все же не обработана.*/
        virtual void show();
        /*! \brief Задавание параметра data
        * \param data - То, что будет использовано в качестве данных
        */
        virtual void set_data(String data);
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
        virtual String get_data();

        virtual ExceptionIdType get_base_exception_id() = 0;
    protected:
        //! \brief Сообщение, которое может быть использовано для отображения сообщения об ошибке
        String data;
        //! \brief id исключения для отличия одних от других
        ExceptionIdType id;
    };
}


#endif // EXCEPTION_H
