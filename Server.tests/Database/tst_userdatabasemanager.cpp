#include "tst_userdatabasemanager.h"
#include "System/keygen.h"

using namespace MafiaTests;
using namespace DatabaseTests;

UserDatabaseManagerTests::UserDatabaseManagerTests(QObject *parent) : QObject(parent)
{
	dbWorker = new Mafia::Database::DatabaseWorker();
	manager = new Mafia::Database::UserDatabaseManager(dbWorker);
	currentUsers = MafiaList<Mafia::System::Tuple<Mafia::Database::User, QString>>();
}

void UserDatabaseManagerTests::add_random_user()
{
	QString email = Mafia::System::KeyGen::generate_key<QString>(20);
	QString nickname = Mafia::System::KeyGen::generate_key<QString>(10);
	QString password = Mafia::System::KeyGen::generate_key<QString>(10);
	QString confirmationKey = "";
	try {
		Mafia::Database::UserIdType id = manager->add_user(nickname, email, password, confirmationKey);

		Mafia::Database::User current = manager->get_user(id);

		QCOMPARE(current.id, id);
		QCOMPARE(current.email, email);
		QCOMPARE(current.nickname, nickname);
		QCOMPARE(current.confirmationKey, confirmationKey);
		QVERIFY(current.authorized == Mafia::Database::AuthorizedStatus_NotAuthorized);
		QVERIFY(current.isConfirmed == Mafia::Database::Status_NotConfirmed);

		currentUsers.append(Mafia::System::Tuple<Mafia::Database::User, QString>(current, password));
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception thrown!");
	}
}

void UserDatabaseManagerTests::add_another_user()
{
	add_random_user();
}

void UserDatabaseManagerTests::add_invalid_user()
{
	QString email = currentUsers[0].item1.email;
	QString nickname = Mafia::System::KeyGen::generate_key<QString>(10);
	QString password = Mafia::System::KeyGen::generate_key<QString>(10);
	QString confirmationKey = "";

	try {
		manager->add_user(nickname, email, password, confirmationKey);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception * exception) {
		//Everything is OK. Exception is thrown
	}
}

void UserDatabaseManagerTests::login_user_correct()
{
	try {
		bool success = manager->login_user(currentUsers[0].item1.id, currentUsers[0].item2);
		Mafia::Database::AuthorizedStatus authorized = manager->get_user(currentUsers[0].item1.id).authorized;
		QVERIFY(authorized == Mafia::Database::AuthorizedStatus_Authorized);
		QVERIFY(success == true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void UserDatabaseManagerTests::login_invalid_user()
{
	try {
		manager->login_user(179, "qwerty");
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::login_user_incorrect()
{
	try {
		bool success = manager->login_user(currentUsers[1].item1.id, "incorrectPassword");
		bool authorized = manager->get_user(currentUsers[1].item1.id).authorized;
		QVERIFY(authorized == Mafia::Database::AuthorizedStatus_NotAuthorized);
		QVERIFY(success == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void UserDatabaseManagerTests::logout_user_correct()
{
	try {
		manager->logout_user(currentUsers[0].item1.id);
		bool authorized = manager->get_user(currentUsers[0].item1.id).authorized;
		QVERIFY(authorized == Mafia::Database::AuthorizedStatus_NotAuthorized);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception thrown!");
	}

}

void UserDatabaseManagerTests::logout_user_incorrect()
{
	try {
		manager->logout_user(179);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK exception is thrown.
	}
}

void UserDatabaseManagerTests::confirm_user_correct()
{
	try {
		bool success = manager->confirm_user(currentUsers[1].item1.id, currentUsers[1].item1.confirmationKey);
		Mafia::Database::Status confirmed = manager->get_user(currentUsers[1].item1.id).isConfirmed;
		QVERIFY(success == true);
		QVERIFY(confirmed = Mafia::Database::Status_Confirmed);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void UserDatabaseManagerTests::confirm_user_incorrect()
{
	try {
		bool success = manager->confirm_user(currentUsers[0].item1.id, "qwert");
		Mafia::Database::Status confirmed = manager->get_user(currentUsers[0].item1.id).isConfirmed;
		QVERIFY(success == false);
		QVERIFY(confirmed = Mafia::Database::Status_NotConfirmed);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void UserDatabaseManagerTests::confirm_invalid_user()
{
	try {
		manager->confirm_user(179, "qwerty");
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::register_game_correct()
{
	auto gameToRegister = Mafia::Gameplay::Game();
	gameToRegister.id = 73;
	gameToRegister.roles = MafiaList<Mafia::Gameplay::Role>()
			<< Mafia::Gameplay::Role_Mafia
			<< Mafia::Gameplay::Role_Sheriff;
	gameToRegister.users = MafiaList<Mafia::Database::UserIdType>()
			<< currentUsers[0].item1.id
			<< currentUsers[1].item1.id;
	gameToRegister.result = Mafia::Gameplay::GameResult_MafiaWins;
	gameToRegister.endingDT = QDateTime::currentDateTimeUtc();
	gameToRegister.beginningDT = QDateTime::currentDateTimeUtc();

	try {
		manager->register_game(gameToRegister);

		Mafia::Database::User firstUser = manager->get_user(currentUsers[0].item1.id);
		Mafia::Database::User secondUser = manager->get_user(currentUsers[1].item1.id);

		QVERIFY(firstUser.allGames.contains(gameToRegister.id));
		QVERIFY(firstUser.victoriesByRoles[Mafia::Gameplay::Role_Mafia] == 1);
		QVERIFY(firstUser.defeatesByRoles[Mafia::Gameplay::Role_Mafia] == 0);

		QVERIFY(secondUser.allGames.contains(gameToRegister.id));
		QVERIFY(secondUser.defeatesByRoles[Mafia::Gameplay::Role_Sheriff] == 1);
		QVERIFY(secondUser.victoriesByRoles[Mafia::Gameplay::Role_Sheriff] == 0);

	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void UserDatabaseManagerTests::register_game_incorrect()
{
	auto gameToRegister = Mafia::Gameplay::Game();
	gameToRegister.id = 12;
	gameToRegister.roles = MafiaList<Mafia::Gameplay::Role>()
			<< Mafia::Gameplay::Role_Mafia
			<< Mafia::Gameplay::Role_Sheriff;
	gameToRegister.users = MafiaList<Mafia::Database::UserIdType>()
			<< 123
			<< 456;
	gameToRegister.result = Mafia::Gameplay::GameResult_MafiaWins;
	gameToRegister.endingDT = QDateTime::currentDateTimeUtc();
	gameToRegister.beginningDT = QDateTime::currentDateTimeUtc();

	try {
		manager->register_game(gameToRegister);
		QFAIL("Exception is not thrown!");
	} catch(Mafia::Exceptions::Exception* exception){
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::get_user_correct()
{
	try {
		Mafia::Database::User firstUser = manager->get_user(currentUsers[0].item1.id);
		QCOMPARE(firstUser.id, currentUsers[0].item1.id);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void UserDatabaseManagerTests::get_user_incorrect()
{
	try {
		Mafia::Database::User firstUser = manager->get_user(789);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::add_transaction_correct()
{
	try {
		Mafia::Database::TransactionIdType id = qrand();
		manager->add_transaction(currentUsers[0].item1.id, id);
		MafiaList<Mafia::Database::TransactionIdType> usersTransactions = manager->get_user(currentUsers[0].item1.id).transactions;

		QVERIFY(usersTransactions.contains(id));
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void UserDatabaseManagerTests::add_transaction_incorrect()
{
	try {
		manager->add_transaction(753, qrand());
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::change_password_correct()
{
	try {
		manager->login_user(currentUsers[0].item1.id, currentUsers[0].item2);

		QString newPassword = Mafia::System::KeyGen::generate_key<QString>(8);
		manager->change_password(currentUsers[0].item1.id, newPassword);
		currentUsers[0].item2 = newPassword;

		manager->logout_user(currentUsers[0].item1.id);
		bool success = manager->login_user(currentUsers[0].item1.id, currentUsers[0].item2);
		manager->logout_user(currentUsers[0].item1.id);
		QVERIFY(success = true);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::change_password_incorrect()
{
	try {
		manager->change_password(currentUsers[0].item1.id, "newPassword");
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::change_password_invalid_user()
{
	try {
		manager->change_password(789, "newPassword");
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::add_achievement_correct()
{
	try {
		manager->add_achievement(currentUsers[0].item1.id, Mafia::Database::Achievement_ProMLGPlayer);

		Mafia::Database::Achievement achievement = manager->get_user(currentUsers[0].item1.id).achievement;
		QCOMPARE(achievement, Mafia::Database::Achievement_ProMLGPlayer);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::add_achievement_incorrect()
{
	try {
		manager->add_achievement(357, Mafia::Database::Achievement_ProMLGPlayer);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::add_user_to_chat_correct()
{
	try {
		Mafia::Database::ChatIdType chatId = qrand();
		manager->add_user_to_chat(currentUsers[0].item1.id, chatId);

		MafiaList<Mafia::Database::ChatIdType> chats = manager->get_user(currentUsers[0].item1.id).chats;
		QVERIFY(chats.contains(chatId));
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}

	currentUsers[0].item1 = manager->get_user(currentUsers[0].item1.id);
	currentUsers[1].item1 = manager->get_user(currentUsers[1].item1.id);
}

void UserDatabaseManagerTests::add_invalid_user_to_chat()
{
	try {
		manager->add_user_to_chat(963, qrand());
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::remove_user_from_chat_correct()
{
	try {
		Mafia::Database::ChatIdType chatId = currentUsers[0].item1.chats[0];
		manager->remove_user_from_chat(currentUsers[0].item1.id, chatId);

		MafiaList<Mafia::Database::ChatIdType> chats = manager->get_user(currentUsers[0].item1.id).chats;
		QVERIFY(!chats.contains(chatId));
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::remove_user_from_chat_incorrect()
{
	try {
		manager->remove_user_from_chat(currentUsers[0].item1.id, qrand());
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::remove_invalid_user_from_chat()
{
	try {
		manager->remove_user_from_chat(852, qrand());
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::set_account_type_correct()
{
	try {
		manager->set_account_type(currentUsers[0].item1.id, Mafia::Database::AccountType_Pro);

		Mafia::Database::AccountType accountType = manager->get_user(currentUsers[0].item1.id).accountType;

		QCOMPARE(accountType, Mafia::Database::AccountType_Pro);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::set_account_type_incorrect()
{
	try {
		manager->set_account_type(726, Mafia::Database::AccountType_Pro);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::change_email_correct()
{
	try {
		QString newEmail = Mafia::System::KeyGen::generate_key<QString>(15);
		manager->change_email(currentUsers[0].item1.id, newEmail);

		currentUsers[0].item1 = manager->get_user(currentUsers[0].item1.id);
		QCOMPARE(newEmail, currentUsers[0].item1.email);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::change_email_incorrect()
{
	try {
		QString newEmail = Mafia::System::KeyGen::generate_key<QString>(15);
		manager->change_email(186, newEmail);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::change_email_copy()
{
	try {
		QString newEmail = currentUsers[1].item1.email;
		bool success = manager->change_email(currentUsers[0].item1.id, newEmail);
		QVERIFY(success == false);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::change_nickname_correct()
{
	try {
		QString newNickname = Mafia::System::KeyGen::generate_key<QString>(10);
		manager->change_nickname(currentUsers[0].item1.id, newNickname);

		currentUsers[0].item1 = manager->get_user(currentUsers[0].item1.id);
		QCOMPARE(newNickname, currentUsers[0].item1.nickname);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::change_nickname_incorrect()
{
	try {
		QString newNickname = Mafia::System::KeyGen::generate_key<QString>(10);
		manager->change_nickname(483, newNickname);

		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//Everything is OK. Exception is thrown.
	}
}

void UserDatabaseManagerTests::add_more_users()
{
	for(int i = 0; i < 10; i++){
		add_random_user();
	}

	manager->login_user(currentUsers[3].item1.id, currentUsers[3].item2);
	manager->login_user(currentUsers[5].item1.id, currentUsers[5].item2);
	manager->login_user(currentUsers[7].item1.id, currentUsers[7].item2);

	manager->add_achievement(currentUsers[2].item1.id, Mafia::Database::Achievement_ProMLGPlayer);
	manager->add_achievement(currentUsers[5].item1.id, Mafia::Database::Achievement_ProMLGPlayer);
	manager->add_achievement(currentUsers[9].item1.id, Mafia::Database::Achievement_ProMLGPlayer);

	manager->confirm_user(currentUsers[7].item1.id, currentUsers[7].item1.confirmationKey);
	manager->confirm_user(currentUsers[1].item1.id, currentUsers[1].item1.confirmationKey);
	manager->confirm_user(currentUsers[8].item1.id, currentUsers[8].item1.confirmationKey);

	manager->change_nickname(currentUsers[3].item1.id, currentUsers[6].item1.nickname);

	for(int i = 0; i < currentUsers.length(); i++){
		currentUsers[i].item1 = manager->get_user(currentUsers[i].item1.id);
	}

}

void UserDatabaseManagerTests::get_users_full_condition()
{
	try {
		MafiaList<Mafia::Database::User> correctUsers = manager->get_users(
			MafiaList<Mafia::Database::UserIdType>() << 2 << 3 << 4 << 5 << 6 << 7 << 9,
			Mafia::Database::Status_NotConfirmed,
			Mafia::Database::Achievement_None,
			Mafia::Database::AuthorizedStatus_Authorized,
			currentUsers[3].item1.nickname);
		QVERIFY(correctUsers.length() == 1);
		QCOMPARE(correctUsers[0], currentUsers[3].item1);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::get_users_empty_condition()
{
	try {
		MafiaList<Mafia::Database::User> allUsers = manager->get_users();
		QVERIFY(allUsers.length() == currentUsers.length());
		for(int i = 0; i < currentUsers.length(); i++){
			QVERIFY(allUsers.contains(currentUsers[i].item1));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::get_users_ids_full_conditions()
{
	try {
		MafiaList<Mafia::Database::UserIdType> correctUserIds = manager->get_users_ids(
			MafiaList<Mafia::Database::UserIdType>() << 2 << 3 << 4 << 5 << 6 << 7 << 9,
			Mafia::Database::Status_NotConfirmed,
			Mafia::Database::Achievement_None,
			Mafia::Database::AuthorizedStatus_Authorized,
			currentUsers[3].item1.nickname);
		QVERIFY(correctUserIds.length() == 1);
		QVERIFY(correctUserIds[0] == currentUsers[3].item1.id);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}

void UserDatabaseManagerTests::get_users_ids_empty_conditions()
{
	try {
		MafiaList<Mafia::Database::UserIdType> allUsers = manager->get_users_ids();
		QVERIFY(allUsers.length() == currentUsers.length());
		for(int i = 0; i < currentUsers.length(); i++){
			QVERIFY(allUsers.contains(currentUsers[i].item1.id));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exceptions is thrown!");
	}
}
