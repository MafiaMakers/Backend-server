#include "databaseworker.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <iterator>
#include "Exceptions/databaseworkingexception.h"
#include <windows.h>
#include "user.h"
#include <iostream>

using namespace Mafia;
using namespace Database;


DatabaseWorker::DatabaseWorker(System::String _path, System::String _backupPath, System::String _filename)
    : path(_path), backupPath(_backupPath), filename(_filename)
{
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
    if(!dataBase.open()){
        std::cout << "Database could not be opened!\n";
    }

    isOnBackup = false;

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
    while(!database_ready()){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        time += 10;
        if(time > maxWaitingTime){
            throw new Exceptions::DatabaseWorkingException(System::String("Too much waiting time"), Exceptions::DatabaseWorkingExceptionId_TimeLimit);
        }
    }

    QSqlQuery* query = new QSqlQuery(dataBase);

    bool success = query->exec(request);
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
    isOnBackup = true;
    std::string fullSourcePath;
    if(path.size > 0){
        fullSourcePath = std::string(path.data, path.size) + '\\' + std::string(filename.data, filename.size);
    } else{
        fullSourcePath = std::string(filename.data, filename.size);
    }
    std::wstring w_fullSourcePath(fullSourcePath.begin(), fullSourcePath.end());

    std::string fullBackupPath = std::string(backupPath.data, backupPath.size) + '\\' + std::string(filename.data, filename.size);
    std::wstring w_fullBackupPath(fullBackupPath.begin(), fullBackupPath.end());

    CopyFile(w_fullBackupPath.c_str(), w_fullSourcePath.c_str(), FALSE);

    isOnBackup = false;
}

void DatabaseWorker::_database_backup()
{
    while(dataBase.isOpen()){
        std::this_thread::sleep_for(std::chrono::milliseconds(backupInterval));
        std::cout << "started backup" << std::endl;
        isOnBackup = true;
        std::string fullSourcePath;
        if(path.size > 0){
            fullSourcePath = std::string(path.data, path.size) + '\\' + std::string(filename.data, filename.size);
        } else{
            fullSourcePath = std::string(filename.data, filename.size);
        }
        std::wstring w_fullSourcePath(fullSourcePath.begin(), fullSourcePath.end());

        std::string fullBackupPath = std::string(backupPath.data, backupPath.size) + '\\' + std::string(filename.data, filename.size);
        std::wstring w_fullBackupPath(fullBackupPath.begin(), fullBackupPath.end());

        CopyFile(w_fullSourcePath.c_str(), w_fullBackupPath.c_str(), FALSE);

        isOnBackup = false;
        std::cout << "ended backup" << std::endl;

    }
}

