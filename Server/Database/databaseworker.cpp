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
        fullPath = QString(path.data) + '\\' + QString(filename.data);
    } else{
        fullPath = QString(filename.data);
    }
    dataBase.setDatabaseName(fullPath);
    dataBase.open();

    isOnBackup = false;

    std::thread backupThread(&DatabaseWorker::_database_backup, this);
    backupThread.detach();
}

bool DatabaseWorker::database_ready()
{
    return dataBase.isOpen() && !isOnBackup;
}

QSqlQuery* DatabaseWorker::run_query(QString request)
{
    int time = 0;
    std::cout << request.toStdString() << std::endl;
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
        throw new Exceptions::DatabaseWorkingException(System::String(exception), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        return 0;
    }

    return query;
}

QString DatabaseWorker::get_sql_type(size_t type_hashcode, int maxListSize)
{
    if(type_hashcode == typeid (int).hash_code()){
        return "integer";
    } else if(type_hashcode == typeid (QString).hash_code()){
        return ("VARCHAR(" + QString::number(STRING_SIZE) + ")");
    } else if(type_hashcode == typeid (bool).hash_code()){
        return "BIT";
    } else if(type_hashcode == typeid (QList<int>).hash_code()){
        return ("VARCHAR(" + QString::number(maxListSize * sizeof(int) * 2)+ ")");
    } else if(type_hashcode == typeid (QDateTime).hash_code()){
        return "DATETIME";
    } else if(type_hashcode == typeid (Status).hash_code()){
        return "integer";
    } else if(type_hashcode == typeid (Achievement).hash_code()){
        return "integer";
    } else if(type_hashcode == typeid (AccountType).hash_code()){
        return "integer";
    } else{
        throw new Exceptions::DatabaseWorkingException(System::String(("Type with unknown hash " + QString::number(type_hashcode)).toStdString()), Exceptions::DatabaseWorkingExceptionId_UnknownType);
    }
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

