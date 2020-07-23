#include "tst_gamedatabasemanager.h"

using namespace MafiaTests;
using namespace DatabaseTests;

GameDatabaseManagerTests::GameDatabaseManagerTests(QObject *parent) : QObject(parent)
{
	dbWorker = new Mafia::Database::DatabaseWorker();
	manager = new Mafia::Database::GameDatabaseManager(dbWorker);

	currentGames = MafiaList<Mafia::Gameplay::Game>();
}

void GameDatabaseManagerTests::add_game_test()
{
	auto currentGame = Mafia::Gameplay::Game();
	currentGame.endingDT = QDateTime::currentDateTimeUtc();
	currentGame.beginningDT = QDateTime::currentDateTimeUtc();
	currentGame.result = (Mafia::Gameplay::GameResult)(qrand() % (int)Mafia::Gameplay::GameResult_Last);

	for(int i = Mafia::Gameplay::Role_First; i < Mafia::Gameplay::Role_Last; i++){
		currentGame.roles.append((Mafia::Gameplay::Role)i);
		currentGame.users.append(qrand());
	}

	try {
		manager->add_game(currentGame);
		currentGames.append(currentGame);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void GameDatabaseManagerTests::add_game_incorrect()
{
	// Не совпадают размеры массива игроков и массива ролей
	auto currentGame = Mafia::Gameplay::Game();
	currentGame.endingDT = QDateTime::currentDateTime();
	currentGame.beginningDT = QDateTime::currentDateTime();
	currentGame.result = (Mafia::Gameplay::GameResult)(qrand() % (int)Mafia::Gameplay::GameResult_Last);
	currentGame.roles = MafiaList<Mafia::Gameplay::Role>() << Mafia::Gameplay::Role_Mafia;
	currentGame.users = MafiaList<Mafia::Database::UserIdType>();


	try {
		manager->add_game(currentGame);
		QFAIL("Exception is not thrown!");
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown
	}
}

void GameDatabaseManagerTests::get_game_data_test()
{
	try {
		Mafia::Gameplay::Game firstGame = manager->get_game_data(currentGames[0].id);

		QCOMPARE(firstGame, currentGames[0]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}

}

void GameDatabaseManagerTests::get_game_data_incorrect()
{
	try {
		manager->get_game_data(123);
	} catch (Mafia::Exceptions::Exception* exception) {
		//OK. Exception is thrown!
	}
}

void GameDatabaseManagerTests::add_more_games()
{
	for(int i = 0; i < 10; i++){
		add_game_test();
	}
}

void GameDatabaseManagerTests::get_games_with_empty_condition()
{
	try {
		MafiaList<Mafia::Gameplay::Game> allGames = manager->get_games_with();
		for(int i = 0; i < allGames.length(); i++){
			QVERIFY(currentGames.contains(allGames[i]));
		}

		for(int i = 0; i < currentGames.length(); i++){
			QVERIFY(allGames.contains(currentGames[i]));
		}
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}

void GameDatabaseManagerTests::get_games_with_full_condition()
{
	try {
		MafiaList<Mafia::Gameplay::Game> correctGames = manager->get_games_with(MafiaList<Mafia::Database::UserIdType>()
				<< currentGames[0].users[0]
				<< currentGames[0].users[1],
			Mafia::Database::FilterType_AND,
			MafiaList<Mafia::Gameplay::Role>() << Mafia::Gameplay::Role_Mafia << Mafia::Gameplay::Role_None,
			Mafia::Database::FilterType_OR,
			MafiaList<Mafia::Gameplay::GameResult>() << currentGames[0].result);

		QVERIFY(correctGames.length() == 1);
		QCOMPARE(correctGames[0], currentGames[0]);
	} catch (Mafia::Exceptions::Exception* exception) {
		exception->show();
		QFAIL("Exception is thrown!");
	}
}
