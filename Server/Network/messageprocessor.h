#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <QObject>
#include "Network/message.h"
#include "Network/mainservernetworker.h"
#include "Network/messageTypes.h"
#include <MafiaDatabase>
#include <MafiaSubservers>

namespace Mafia {
    namespace Network {
        class MessageProcessor : public QObject
        {
            Q_OBJECT
        public:
            explicit MessageProcessor(QObject *parent = nullptr);

            MessageProcessor(Network::MainServerNetworker* networker);

            static MessageProcessor* instance;

        signals:
            void create_user(QString nickname, QString email, QString password, Network::Client client, Network::MessageIdType requestId);

            void send_chat_message(Network::Client sender, Database::ChatIdType toChat, QString data, QList<Database::MessageIdType> answerFor,
                                   Database::ChatFeature feature);

            void login_user(QString email, QString password, Network::Client client, Network::MessageIdType requestId);

            void create_chat(Network::Client creator, Network::MessageIdType requestId);

            void get_last_messages(Network::Client client, Database::ChatIdType chat, Network::MessageIdType requestId, int messagesCount = -1);

            void get_users_chats(Network::Client client, Network::MessageIdType requestId, int chatsCount);

            void add_user_to_chat(Database::ChatIdType chat, Database::UserIdType user, Network::Client initializer,
                                  Database::ChatCapability capability = Database::ChatCapabilities_Speaker);

            void remove_user_from_chat(Database::ChatIdType chat, Database::UserIdType user, Network::Client initializer);

            void change_users_chat_capability(Database::ChatIdType chat, Database::UserIdType user,
                                              Database::ChatCapability newCapability, Network::Client initializer);

            void create_game(Network::Client creator);

            void get_statistics(Database::UserIdType user, Network::Client asker, Network::MessageIdType requestId);

            void confirm_email(Network::Client client, QString confirmationKey, MessageIdType requestId);

            void logout_user(Network::Client client);

            void get_logs_data(QString data, Network::Client sender);

            void change_nickname(Network::Client client, QString newNickname);

            void change_email(Network::Client client, QString newEmail, Network::MessageIdType requestId);

            void add_user_to_game(Network::Client client, QString gameKey);

            void delete_message(Network::Client client, Database::MessageIdType message);

            void edit_message(Network::Client client, Database::ChatMessage message);

            void read_message(Network::Client client, Database::MessageIdType message);

        private slots:

            void message_received(Network::Message message);

        private:

            Network::MainServerNetworker* networker;

        };
    }

}


#endif // MESSAGEPROCESSOR_H
