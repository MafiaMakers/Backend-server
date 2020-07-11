#ifndef SERIALIZERTESTS_H
#define SERIALIZERTESTS_H
#include <QTest>
#include <QObject>
#include "System/serializer.h"
//#include "System/serializer.cpp"

namespace MafiaTests {
    namespace SystemTests {
        class SerializerTests : public QObject
        {
            Q_OBJECT
        public:
            explicit SerializerTests(QObject *parent = nullptr);
            ~SerializerTests();

        private slots:
            void int_11_11_returns();
            void deserialize_short_message_int_error_returns();


        };
    }
}


#endif // SERIALIZERTESTS_H
