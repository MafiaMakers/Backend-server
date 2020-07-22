#ifndef USERDATABASEMANAGERTESTS_H
#define USERDATABASEMANAGERTESTS_H
#include "Database/user.h"
#include "Database/userdatabasemanager.h"
#include "System/tuple.h"

#include <QtTest>
#include <QObject>

namespace MafiaTests {
	namespace DatabaseTests {
		class UserDatabaseManagerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit UserDatabaseManagerTests(QObject *parent = nullptr);

		private slots:
			void add_random_user();
			void add_another_user();
			void add_invalid_user();
			void login_user_correct();
			void login_invalid_user();
			void login_user_incorrect();
			void logout_user_correct();
			void logout_user_incorrect();
			void confirm_user_correct();
			void confirm_user_incorrect();
			void confirm_invalid_user();
			void register_game_correct();
			void register_game_incorrect();
			void get_user_correct();
			void get_user_incorrect();
			void add_transaction_correct();
			void add_transaction_incorrect();
			void change_password_correct();
			void change_password_incorrect();
			void change_password_invalid_user();
			void add_achievement_correct();
			void add_achievement_incorrect();
			void add_user_to_chat_correct();
			void add_invalid_user_to_chat();
			void remove_user_from_chat_correct();
			void remove_user_from_chat_incorrect();
			void remove_invalid_user_from_chat();
			void set_account_type_correct();
			void set_account_type_incorrect();
			void change_email_correct();
			void change_email_incorrect();
			void change_email_copy();
			void change_nickname_correct();
			void change_nickname_incorrect();
			void add_more_users();
			void get_users_full_condition();
			void get_users_empty_condition();
			void get_users_ids_full_conditions();
			void get_users_ids_empty_conditions();
		private:
			MafiaList<Mafia::System::Tuple<Mafia::Database::User, QString>> currentUsers;
			Mafia::Database::UserDatabaseManager* manager;
			Mafia::Database::DatabaseWorker* dbWorker;

		};
	}
}


#endif // USERDATABASEMANAGERTESTS_H
