#ifndef GAMEDATABASEMANAGERTESTS_H
#define GAMEDATABASEMANAGERTESTS_H
#include <QTest>
#include <QObject>
#include "Database/gamedatabasemanager.h"
#include "Exceptions/exception.h"

namespace MafiaTests {
	namespace DatabaseTests {
		class GameDatabaseManagerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit GameDatabaseManagerTests(QObject *parent = nullptr);

		private slots:
			void add_game_test();
			void add_game_incorrect();
			void get_game_data_test();
			void get_game_data_incorrect();
			void add_more_games();
			void get_games_with_empty_condition();
			void get_games_with_full_condition();

		private:
			Mafia::Database::DatabaseWorker* dbWorker;
			Mafia::Database::GameDatabaseManager* manager;

			MafiaList<Mafia::Gameplay::Game> currentGames;
		};
	}
}


#endif // GAMEDATABASEMANAGERTESTS_H
