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
             * \brief Функция десериализации превращает строку с данными в данные. При этом изменяет саму строку, сдвигая ее начало
             * Чтобы можно было несколько переменных подряд десериализовывать
             * \param data Строка с исходными данными (может содержать данные и после тех, что требуется извлечь - они не будут
             * удалены)
             * \return Данные в требуемом виде
             */
            static T deserialize (String &data);
        };
    }
}


#endif // SERIALIZER_H
