#ifndef TST_TUPLE_H
#define TST_TUPLE_H

#include <QTest>
#include "System/tuple.h"

namespace MafiaTests {
    namespace SystemTests {
        class TupleTests : public QObject
        {
            Q_OBJECT

        public:
            TupleTests();
            ~TupleTests();

        private slots:
            void int_int_tuple_equality_equal();

            void int_int_tuple_equality_not_equal();

            void qstring_double_equality_equal();

            void qstring_double_equality_not_equal();

            void int_int_tuple_inequality_equal();

            void int_int_tuple_inequality_not_equal();

            void qstring_double_inequality_equal();

            void qstring_double_inequality_not_equal();

        };
    }
}

#endif // TST_TUPLE_H
