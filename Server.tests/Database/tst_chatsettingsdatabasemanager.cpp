#include "tst_chatsettingsdatabasemanager.h"

using namespace MafiaTests;
using namespace DatabaseTests;

ChatSettingsDatabaseManagerTests::ChatSettingsDatabaseManagerTests(QObject *parent) : QObject(parent)
{
	dbWorker = new Mafia::Database::DatabaseWorker();
	manager = new Mafia::Database::ChatSettingsDatabaseManager(dbWorker);

	currentChats = MafiaList<Mafia::Database::Chat>();
	usersCount = 0;
}

void ChatSettingsDatabaseManagerTests::create_chat_test()
{
	try {
		auto currentChat = Mafia::Database::Chat();
		currentChat.id = manager->create_chat();
		currentChat.users = MafiaList<Mafia::Database::UserIdType>();
		currentChat.creationTime = QDateTime::currentDateTimeUtc();
		currentChat.usersCapabilities = MafiaList<Mafia::Database::ChatCapability>();
		currentChats.append(currentChat);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatSettingsDatabaseManagerTests::add_user_to_chat_correct_default()
{
	try {
		manager->add_user_to_chat(usersCount, currentChats[0].id);
		currentChats[0].users.append(usersCount);
		currentChats[0].usersCapabilities.append(Mafia::Database::ChatCapabilities_Speaker);
		usersCount++;

		Mafia::Database::Chat chat = manager->get_chat(currentChats[0].id);
		QCOMPARE(chat, currentChats[0]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatSettingsDatabaseManagerTests::add_user_to_chat_capability()
{
	try {
		manager->add_user_to_chat(usersCount, currentChats[0].id, Mafia::Database::ChatCapabilities_Admin);
		currentChats[0].users.append(usersCount);
		currentChats[0].usersCapabilities.append(Mafia::Database::ChatCapabilities_Admin);
		usersCount++;

		Mafia::Database::Chat chat = manager->get_chat(currentChats[0].id);
		QCOMPARE(chat, currentChats[0]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::add_user_to_invalid_chat()
{
	try {
		manager->add_user_to_chat(qrand(), 123);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::add_more_chats()
{
	for(int i = 0; i < 5; i++){
		create_chat_test();
	}
}

void ChatSettingsDatabaseManagerTests::add_more_users()
{
	for(int i = 0; i < currentChats.length(); i++){
		for(int j = Mafia::Database::ChatCapabilities_First; j < Mafia::Database::ChatCapabilities_Last; j++){
			manager->add_user_to_chat(usersCount, currentChats[i].id, (Mafia::Database::ChatCapability)j);
			currentChats[i].users.append(usersCount);
			currentChats[i].usersCapabilities.append((Mafia::Database::ChatCapability)j);
			usersCount++;
		}
	}
}

void ChatSettingsDatabaseManagerTests::get_chats_with_empty_condition()
{
	try {
		MafiaList<Mafia::Database::ChatIdType> allChats = manager->get_chats_with();
		for(int i = 0; i < currentChats.length(); i++){
			QVERIFY(allChats.contains(currentChats[i].id));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::get_chats_with_full_condition()
{
	try {
		MafiaList<Mafia::Database::ChatIdType> correctChats = manager->get_chats_with(
			MafiaList<Mafia::Database::ChatIdType>() << currentChats[0].id << currentChats[1].id,
			MafiaList<Mafia::Database::UserIdType>() << currentChats[0].users[0] << currentChats[0].users[1],
			Mafia::Database::FilterType_OR
		);

		QVERIFY(correctChats.length() == 1);
		QCOMPARE(correctChats[0], currentChats[0].id);

	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::get_chats_with_invalid_condition()
{
	try {
		manager->get_chats_with(
			MafiaList<Mafia::Database::ChatIdType>() << qrand() << qrand(),
			MafiaList<Mafia::Database::ChatIdType>() << qrand() << qrand());
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Excepton is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::remove_user_from_chat_correct()
{
	try {
		manager->remove_user_from_chat(currentChats[0].users[0], currentChats[0].id);
		currentChats[0].users.removeAt(0);
		currentChats[0].usersCapabilities.removeAt(0);

		Mafia::Database::Chat firstChat = manager->get_chat(currentChats[0].id);
		QCOMPARE(firstChat, currentChats[0]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatSettingsDatabaseManagerTests::remove_user_from_chat_incorrect()
{
	try {
		manager->remove_user_from_chat(234, currentChats[0].id);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::remove_user_from_invalid_chat()
{
	try {
		manager->remove_user_from_chat(234, 345);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::get_all_chats_test()
{
	try {
		MafiaList<Mafia::Database::Chat> allChats = manager->get_all_chats();

		QCOMPARE(allChats.length(), currentChats.length());
		for(int i = 0; i < allChats.length(); i++){
			QVERIFY(currentChats.contains(allChats[i]));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatSettingsDatabaseManagerTests::get_users_chats_correct()
{
	try {
		MafiaList<Mafia::Database::Chat> usersChats = manager->get_users_chat(currentChats[1].users[0]);
		QVERIFY(usersChats.length() == 1);
		QCOMPARE(usersChats[0], currentChats[1]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatSettingsDatabaseManagerTests::get_users_chats_empty()
{
	try {
		MafiaList<Mafia::Database::Chat> emptyUsersChats = manager->get_users_chat(456);
		QVERIFY(emptyUsersChats.length() == 0);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatSettingsDatabaseManagerTests::can_send_message_correct()
{
	try {
		bool canSend = manager->can_send_message(currentChats[1].users[Mafia::Database::ChatCapabilities_Admin], currentChats[1].id);
		QVERIFY(canSend == true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_send_message_invalid_user()
{
	try {
		bool canSend = manager->can_send_message(567, currentChats[1].id);
		QVERIFY(canSend == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_send_message_weak_capability()
{
	try {
		bool canSend = manager->can_send_message(currentChats[1].users[Mafia::Database::ChatCapabilities_Watcher], currentChats[1].id);
		QVERIFY(canSend == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_send_message_invalid_chat()
{
	try {
		manager->can_send_message(currentChats[1].users[Mafia::Database::ChatCapabilities_Admin], 678);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown!
	}
}

void ChatSettingsDatabaseManagerTests::can_read_message_correct()
{
	try {
		bool canRead = manager->can_read_message(currentChats[2].users[Mafia::Database::ChatCapabilities_Watcher], currentChats[2].id);
		QVERIFY(canRead == true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_read_message_invalid_user()
{
	try {
		bool canRead = manager->can_read_message(789, currentChats[2].id);
		QVERIFY(canRead == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_read_message_hard_user()
{
	try {
		bool canRead = manager->can_read_message(currentChats[2].users[Mafia::Database::ChatCapabilities_Admin], currentChats[2].id);
		QVERIFY(canRead == true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_read_message_invalid_chat()
{
	try {
		manager->can_read_message(currentChats[2].users[Mafia::Database::ChatCapabilities_Watcher], 8901);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_users_correct()
{
	try {
		bool canEdit = manager->can_edit_users(currentChats[3].users[Mafia::Database::ChatCapabilities_Admin], currentChats[3].id);
		QVERIFY(canEdit == true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_users_invalid_user()
{
	try {
		bool canEdit = manager->can_edit_users(890, currentChats[3].id);
		QVERIFY(canEdit == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_users_weak_capability()
{
	try {
		bool canEdit = manager->can_edit_users(currentChats[3].users[Mafia::Database::ChatCapabilities_Speaker], currentChats[3].id);
		QVERIFY(canEdit == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_users_invalid_chat()
{
	try {
		manager->can_edit_users(currentChats[3].users[Mafia::Database::ChatCapabilities_Admin], 901);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Ok. Exception is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_messages_correct()
{
	try {
		bool canEdit = manager->can_edit_messages(currentChats[4].users[Mafia::Database::ChatCapabilities_Editor], currentChats[4].id);
		QVERIFY(canEdit == true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_messages_invalid_user()
{
	try {
		bool canEdit = manager->can_edit_messages(1234, currentChats[4].id);
		QVERIFY(canEdit == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_messages_weak_capability()
{
	try {
		bool canEdit = manager->can_edit_messages(currentChats[4].users[Mafia::Database::ChatCapabilities_Speaker], currentChats[4].id);
		QVERIFY(canEdit == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::can_edit_messages_invalid_chat()
{
	try {
		manager->can_edit_messages(currentChats[4].users[Mafia::Database::ChatCapabilities_Editor], 2345);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatSettingsDatabaseManagerTests::get_capability_correct()
{
	try {
		Mafia::Database::ChatCapability capability = manager->get_capability(
			currentChats[4].users[Mafia::Database::ChatCapabilities_Editor],
			currentChats[4].id);

		QCOMPARE(capability, Mafia::Database::ChatCapabilities_Editor);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::get_capability_invalid_user()
{
	try {
		Mafia::Database::ChatCapability capability = manager->get_capability(
			3456,
			currentChats[4].id);

		QCOMPARE(capability, Mafia::Database::ChatCapabilities_None);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatSettingsDatabaseManagerTests::get_capability_invalid_chat()
{
	try {
		manager->get_capability(
			currentChats[4].users[Mafia::Database::ChatCapabilities_Editor],
			4567);

		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}
