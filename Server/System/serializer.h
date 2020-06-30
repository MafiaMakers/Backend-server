#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "functions.h"
#include "simplestring.h"
#include <QString>
namespace Mafia {
    namespace System {
        /*!
         * \brief Класс (по сути статический. Наверное, можно вообще чисто методы оставить, а класс убрать...) для сериализации данных для передачи их по сети
         */
        class Serializer
        {
        public:
            /*!
             * \brief Serializer
             */
            Serializer();


            template<class T>
            /*!
             * \brief Функция сериализации (принимает данные, преобразует их в std::string,
             * содержащюю достаточно данных для восстановления изначальных данных)
             * \param value Данные, которые необходимо сериализовать
             * \return Строку, готовую для вставления в сообщение
             */
            static std::string serialize(T value);

            template<class T>
            /*!
             * \brief Функция десериализации. Соответственно, принимает полученную из сообщения сериализованную строку и
             * преобразует ее в необходимые данные
             * \param data Данные в строке
             * \return Необходимую структуру данных
             */
            static T deserialize (String data)
            {
                return deserialize<T>(data, NULL);
            }

        private:
            template<class T>
            /*!
             * \brief Это тоже функция десериализации, но она еще изменяет значение указателя на индекс
             * (это используется для того, чтобы при десериализации подтипов, например, если в структуре есть поля,
             * каждое из которых надо десериализовать, знать, сколько байт от строки прочитал десериализатор этого одного поля,
             * и далее десериализовывать уже только остаток)
             * \param data Все те же данные
             * \param pointerIndex Указатель на индекс текущего символа в общей строке сериализации более крупной структуры
             * (Если он равен NULL, то ничего не изменяется)
             * \return Все так же необходимую десериализованную структуру данных
             */
            static T deserialize (String data, unsigned int* pointerIndex);

        };
    }
}


#endif // SERIALIZER_H
