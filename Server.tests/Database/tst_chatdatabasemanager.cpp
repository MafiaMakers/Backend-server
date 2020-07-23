#include "tst_chatdatabasemanager.h"
#include "Exceptions/exception.h"
#include "System/keygen.h"

using namespace MafiaTests;
using namespace DatabaseTests;

ChatDatabaseManagerTests::ChatDatabaseManagerTests(QObject *parent) : QObject(parent)
{
	dbWorker = new Mafia::Database::DatabaseWorker();
	manager = new Mafia::Database::ChatDatabaseManager(dbWorker);
}

void ChatDatabaseManagerTests::get_last_messages_empty()
{
	try {
		MafiaList<Mafia::Database::ChatMessage> lastMessagesEmpty = manager->get_last_messages(qrand());
		QVERIFY(lastMessagesEmpty.length() == 0);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::add_message_correct()
{
	try {
		auto messageToAdd = Mafia::Database::ChatMessage();
		messageToAdd.data = Mafia::System::KeyGen::generate_key<QString>(50);
		messageToAdd.from = qrand();
		messageToAdd.toChat = 0;
		messageToAdd.feature = (Mafia::Database::ChatFeature)(qrand() % 3);
		messageToAdd.answerFor = MafiaList<Mafia::Database::MessageIdType>() << qrand() << qrand();
		messageToAdd.readUsers = MafiaList<Mafia::Database::UserIdType>();
		messageToAdd.id = manager->add_message(messageToAdd);
		messageToAdd.timestamp = QDateTime::currentDateTimeUtc();

		Mafia::Database::ChatMessage message = manager->get_message(messageToAdd.id);
		currentMessages.append(messageToAdd);
		QCOMPARE(message, messageToAdd);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::add_more_messages()
{
	for(int i = 0; i < 5; i++){
		add_message_correct();
	}
}

void ChatDatabaseManagerTests::delete_message_correct()
{
	try {
		manager->delete_message(currentMessages[0].id);
		QVERIFY_EXCEPTION_THROWN(manager->get_message(currentMessages[0].id), Mafia::Exceptions::Exception*);

		currentMessages.removeAt(0);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::delete_message_incorrect()
{
	try {
		manager->delete_message(4567);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatDatabaseManagerTests::edit_message_correct()
{
	try {
		currentMessages[2].answerFor.append(qrand());
		currentMessages[2].data = Mafia::System::KeyGen::generate_key<QString>(40);

		manager->edit_message(currentMessages[2]);

		currentMessages[2].feature = Mafia::Database::ChatFeature_Edited;
		currentMessages[2].readUsers = MafiaList<Mafia::Database::UserIdType>();
		currentMessages[2].timestamp = QDateTime::currentDateTimeUtc();

		Mafia::Database::ChatMessage message = manager->get_message(currentMessages[2].id);
		QCOMPARE(message, currentMessages[2]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::edit_message_incorrect()
{
	try {
		Mafia::Database::ChatMessage tempEditedMessage = currentMessages[3];

		tempEditedMessage.from = qrand();
		tempEditedMessage.toChat = qrand();

		manager->edit_message(tempEditedMessage);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown!
	}
}

void ChatDatabaseManagerTests::message_read_correct()
{
	try {
		Mafia::Database::UserIdType readUser = qrand();
		manager->message_read(currentMessages[0].id, readUser);
		if(!currentMessages[0].readUsers.contains(readUser)){
			currentMessages[0].readUsers.append(readUser);
		}

		Mafia::Database::ChatMessage message = manager->get_message(currentMessages[0].id);
		QVERIFY(message.readUsers.contains(readUser));
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::message_read_incorrect()
{
	try {
		manager->message_read(5678, qrand());
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatDatabaseManagerTests::get_messages_empty_condition()
{
	try {
		MafiaList<Mafia::Database::ChatMessage> allMessages = manager->get_messages();

		QCOMPARE(allMessages.length(), currentMessages.length());
		for(int i = 0; i < currentMessages.length(); i++){
			QVERIFY(allMessages.contains(currentMessages[i]));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::get_messages_full_condition()
{
	try {
		MafiaList<Mafia::Database::ChatMessage> correctMessages = manager->get_messages(
			MafiaList<Mafia::Database::ChatIdType>() << currentMessages[0].id << currentMessages[1].toChat
				<< currentMessages[2].toChat << currentMessages[3].toChat,
			MafiaList<Mafia::Database::UserIdType>() << currentMessages[0].from << currentMessages[1].from,
			MafiaList<Mafia::Database::ChatFeature>() << currentMessages[0].feature,
			currentMessages[0].data
		);
		QVERIFY(correctMessages.length() == 1);
		QCOMPARE(correctMessages[0], currentMessages[0]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void ChatDatabaseManagerTests::get_message_correct()
{
	try {
		Mafia::Database::ChatMessage message = manager->get_message(currentMessages[1].id);
		QCOMPARE(message, currentMessages[1]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::get_message_incorrect()
{
	try {
		manager->get_message(6789);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}

void ChatDatabaseManagerTests::get_last_messages_less_than_default()
{
	try {
		MafiaList<Mafia::Database::ChatMessage> chatMessages = manager->get_last_messages(0);

		QVERIFY(chatMessages.length() == currentMessages.length());

		for(int i = 0; i < currentMessages.length(); i++){
			QVERIFY(chatMessages.contains(currentMessages[i]));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::get_last_messages_customized_count()
{
	try {
		MafiaList<Mafia::Database::ChatMessage> chatMessages = manager->get_last_messages(0, 1);

		QVERIFY(chatMessages.length() == 1);
		QVERIFY(currentMessages.contains(chatMessages[0]));

		for(int i = 0; i < currentMessages.length(); i++){
			//std::cout << currentMessages[i].timestamp.toString().toStdString() << "\n"
			//	<< chatMessages[0].timestamp.toString().toStdString() << "\n";
			QVERIFY(currentMessages[i].timestamp.msecsTo(chatMessages[0].timestamp) < 1000);
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void ChatDatabaseManagerTests::get_last_messages_incorrect_chat()
{
	try {
		MafiaList<Mafia::Database::ChatMessage> emptyMessages = manager->get_last_messages(7890);
		QVERIFY(emptyMessages.length() == 0);
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown.
	}
}
