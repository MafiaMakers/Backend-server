#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "functions.h"
#include "simplestring.h"
#include <QString>
namespace Mafia {
    namespace System {
        class Serializer
        {
        public:
            Serializer();

            template<class T>
            static std::string serialize(T value);

            template<class T>
            static T deserialize (String data);
        };
    }
}


#endif // SERIALIZER_H
