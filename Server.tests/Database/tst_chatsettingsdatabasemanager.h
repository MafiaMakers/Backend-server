#ifndef CHATSETTINGSDATABASEMANAGERTESTS_H
#define CHATSETTINGSDATABASEMANAGERTESTS_H
#include <QTest>
#include <QObject>

#include "Database/chatsettingsdatabasemanager.h"

namespace MafiaTests {
	namespace DatabaseTests {
		class ChatSettingsDatabaseManagerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit ChatSettingsDatabaseManagerTests(QObject *parent = nullptr);

		private slots:
			void create_chat_test();

			void add_user_to_chat_correct_default();
			void add_user_to_chat_capability();
			void add_user_to_invalid_chat();

			void add_more_chats();
			void add_more_users();

			void get_chats_with_empty_condition();
			void get_chats_with_full_condition();
			void get_chats_with_invalid_condition();

			void remove_user_from_chat_correct();
			void remove_user_from_chat_incorrect();
			void remove_user_from_invalid_chat();

			void get_all_chats_test();

			void get_users_chats_correct();
			void get_users_chats_empty();

			void can_send_message_correct();
			void can_send_message_invalid_user();
			void can_send_message_weak_capability();
			void can_send_message_invalid_chat();

			void can_read_message_correct();
			void can_read_message_invalid_user();
			void can_read_message_hard_user();
			void can_read_message_invalid_chat();

			void can_edit_users_correct();
			void can_edit_users_invalid_user();
			void can_edit_users_weak_capability();
			void can_edit_users_invalid_chat();

			void can_edit_messages_correct();
			void can_edit_messages_invalid_user();
			void can_edit_messages_weak_capability();
			void can_edit_messages_invalid_chat();

			void get_capability_correct();
			void get_capability_invalid_user();
			void get_capability_invalid_chat();

		private:
			Mafia::Database::DatabaseWorker* dbWorker;
			Mafia::Database::ChatSettingsDatabaseManager* manager;

			MafiaList<Mafia::Database::Chat> currentChats;
			int usersCount;

		};
	}
}


#endif // CHATSETTINGSDATABASEMANAGERTESTS_H
