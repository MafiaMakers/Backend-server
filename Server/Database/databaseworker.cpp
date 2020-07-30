#include "databaseworker.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <iterator>
#include "Exceptions/databaseworkingexception.h"
#include <windows.h>
#include "user.h"
#include "System/logsmanager.h"
#include <iostream>

using namespace Mafia;
using namespace Database;


DatabaseWorker::DatabaseWorker(System::String _path, System::String _backupPath, System::String _filename)
    : path(_path), backupPath(_backupPath), filename(_filename)
{
	restartAttempts = 0;
	//Запускаем БД
	restart_database();
	//Запускаем в отдельный поток резервное копирование БД
    std::thread backupThread(&DatabaseWorker::_database_backup, this);
    backupThread.detach();
}

bool DatabaseWorker::database_ready()
{
    return dataBase.isOpen() && !isOnBackup;
}

bool DatabaseWorker::database_open()
{
    return dataBase.isOpen();
}

QSqlQuery* DatabaseWorker::run_query(QString request)
{
    int time = 0;
    //std::cout << request.toStdString() << std::endl;

	//Ждем, когда БД будет готова к использованию (обычно она сразу готова, но всякое может быть -
	//может быть резервное копирование или еще что-то... в это время лучше не беспокоить файл)
    while(!database_ready()){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        time += 10;
		//Но если ждем слишком долго, то это плохой знак
        if(time > maxWaitingTime){
            throw new Exceptions::DatabaseWorkingException(System::String("Too much waiting time"), Exceptions::DatabaseWorkingExceptionId_TimeLimit);
        }
    }

    QSqlQuery* query = new QSqlQuery(dataBase);

    bool success = query->exec(request);
	//Если провалились, то кидаем исключение
    if(!success){
        std::string exception = "Unable to execute query\n";
        exception += query->lastError().text().toStdString();
        exception += "\n" + request.toStdString();
        throw new Exceptions::DatabaseWorkingException(System::String(exception), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        return 0;
    }

    return query;
}

void DatabaseWorker::restore_database()
{
	//Приостанавливаем работу с БД
    isOnBackup = true;

	//Собираем путь к резервной копии БД и к нужному файлу
    std::string fullSourcePath;
    if(path.size > 0){
        fullSourcePath = std::string(path.data, path.size) + '\\' + std::string(filename.data, filename.size);
    } else{
        fullSourcePath = std::string(filename.data, filename.size);
    }
    std::wstring w_fullSourcePath(fullSourcePath.begin(), fullSourcePath.end());

    std::string fullBackupPath = std::string(backupPath.data, backupPath.size) + '\\' + std::string(filename.data, filename.size);
	std::wstring w_fullBackupPath(fullBackupPath.begin(), fullBackupPath.end());

	//Копируем
    CopyFile(w_fullBackupPath.c_str(), w_fullSourcePath.c_str(), FALSE);

	System::LogsManager::add_record("Database has been restored",
									System::LogType_Message,
									System::LogSource_MainServer,
									"");
	//Возобнавляем работу
	restart_database();
	isOnBackup = false;
}

bool DatabaseWorker::restart_database()
{
	//Приостанавливаем работу
	isOnBackup = true;
	restartAttempts++;

	//Инициализируем
	dataBase = QSqlDatabase::addDatabase("QSQLITE");
	QString fullPath;
	if(path.size > 0){
		fullPath = QString::fromStdString(std::string(path.data, path.size)) + '\\' +
				QString::fromStdString(std::string(filename.data, filename.size));
	} else{
		fullPath = QString::fromStdString(std::string(filename.data, filename.size));
	}

	dataBase.setDatabaseName(fullPath);
	dataBase.setUserName("Admin");
	dataBase.setPassword("admin");

	//Пробуем открыть
	bool success = dataBase.open();

	if(!success){
		std::cout << "Database could not be opened!\n";
	} else {
		restartAttempts = 0;
	}

	isOnBackup = false;

	//Если превысили лимит перезапусков, то все очень плохо, кидаем ошибку
	if(restartAttempts > maxRestartAttempts){
		restartAttempts = 0;
		throw new Exceptions::DatabaseWorkingException(
			System::String("Restart attempts limit overflowed!"),
			Exceptions::DatabaseWorkingExceptionId_DatabaseRestartAttemptsLimit
			);
	}

	return success;
}

void DatabaseWorker::_database_backup()
{
    while(dataBase.isOpen()){
        std::this_thread::sleep_for(std::chrono::milliseconds(backupInterval));
		//std::cout << "started backup" << std::endl;

		//Приостанавливаем работу
        isOnBackup = true;

		//Собираем путь к БД и к файлу резервной копии
        std::string fullSourcePath;
        if(path.size > 0){
            fullSourcePath = std::string(path.data, path.size) + '\\' + std::string(filename.data, filename.size);
        } else{
            fullSourcePath = std::string(filename.data, filename.size);
        }
        std::wstring w_fullSourcePath(fullSourcePath.begin(), fullSourcePath.end());

        std::string fullBackupPath = std::string(backupPath.data, backupPath.size) + '\\' + std::string(filename.data, filename.size);
        std::wstring w_fullBackupPath(fullBackupPath.begin(), fullBackupPath.end());
		//Копируем
        CopyFile(w_fullSourcePath.c_str(), w_fullBackupPath.c_str(), FALSE);
		//Возобнавляем работу
        isOnBackup = false;
		//std::cout << "ended backup" << std::endl;

    }
}

