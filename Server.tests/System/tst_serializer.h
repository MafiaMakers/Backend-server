#ifndef SERIALIZERTESTS_H
#define SERIALIZERTESTS_H
#include <QTest>
#include <QObject>
#include "System/serializer.h"
#include "Database/chatmessage.h"
#include "userstatistics.h"
#include "Gameplay/game_s.h"
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
            void integer();
            void deserialize_short_message_int_error_returns();
            void client();
            void chat_capability();
            void list_of_int();
            void character();
            void boolean();
            void list_of_char();
            void qstring();
            void qdatetime();
            void chat_message();
            void list_of_chat_message();
            void user_statistics();
            void tuple_of_user_id_and_qstring();
            void client_info();
            void list_of_roles();
            void game();
            void tuple_of_int_and_int();
            void network_message();
            void tuple_of_network_message_and_int();

        private:
            Mafia::Database::ChatMessage generate_random_message();
            Mafia::UserStatistics generate_random_statistics();
            Mafia::Network::Message generate_random_network_message();
        };
    }
}

#endif // SERIALIZERTESTS_H
