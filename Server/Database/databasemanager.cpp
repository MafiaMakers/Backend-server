#include "databasemanager.h"
using namespace Mafia;
using namespace Database;


DatabaseManager::DatabaseManager(DatabaseWorker *databaseWorker, QString name) : dbName(name)
{
    dbWorker = databaseWorker;
}
