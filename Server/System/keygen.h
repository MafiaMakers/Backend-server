#ifndef KEYGEN_H
#define KEYGEN_H
#include "functions.h"
#include "simplestring.h"

namespace Mafia {
    namespace System {
        //! \brief По сути статический класс для случайной генерации различных ключей (солей в т.ч.)
        class KeyGen
        {
        public:
            template<class T>
            /*!
             * \brief Функция случайной генерации строки из латинских символов
             * Шаблон может принимать типы QString, std::string, Mafia::System::String
             * \param length Длина строки
             * \return Сгенерированную строку
             */
            static T generate_key(int length);
        };
    }
}



#endif // KEYGEN_H
