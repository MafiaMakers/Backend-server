#ifndef CHATDATABASEMANAGERTESTS_H
#define CHATDATABASEMANAGERTESTS_H
#include <QTest>
#include <QObject>

#include "Database/chatdatabasemanager.h"

namespace MafiaTests {
	namespace DatabaseTests {
		class ChatDatabaseManagerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit ChatDatabaseManagerTests(QObject *parent = nullptr);

		private slots:
			void get_last_messages_empty();

			void add_message_correct();
			void add_more_messages();

			void delete_message_correct();
			void delete_message_incorrect();

			void edit_message_correct();
			void edit_message_incorrect();

			void message_read_correct();
			void message_read_incorrect();

			void get_messages_empty_condition();
			void get_messages_full_condition();

			void get_message_correct();
			void get_message_incorrect();

			void get_last_messages_less_than_default();
			void get_last_messages_customized_count();
			void get_last_messages_incorrect_chat();



		private:
			Mafia::Database::DatabaseWorker* dbWorker;
			Mafia::Database::ChatDatabaseManager* manager;

			MafiaList<Mafia::Database::ChatMessage> currentMessages;

		};
	}
}
#endif // CHATDATABASEMANAGERTESTS_H
