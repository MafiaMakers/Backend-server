#include "transactiondatabasemanager.h"
#include "Exceptions/databaseworkingexception.h"
#include "databasehelper.h"
#include "System/SHA_256.h"
#include <thread>
#include <chrono>

using namespace Mafia;
using namespace Database;

//const QString TransactionDatabaseWorker::dbName = "transactions_db";
const int TransactionDatabaseManager::hashCheckingInterval = 30000;


TransactionDatabaseManager::TransactionDatabaseManager(DatabaseWorker* databaseWorker) : DatabaseManager(databaseWorker, "transactions_db")
{
    try {
        dbWorker->run_query("SELECT * FROM " + dbName);
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
            QString request = "CREATE TABLE " + dbName + " ("
                    "ID " + get_sql_type<TransactionIdType>() + " PRIMARY KEY" + NOT_NULL + ", "
                    "USER_ID " + get_sql_type<UserIdType>() + NOT_NULL + ", "
                    "PRICE " + get_sql_type<PriceType>() + NOT_NULL + ", "
                    "TYPE " + get_sql_type<TransactionType>() + NOT_NULL + ", "
                    "HASH " + get_sql_type<QString>() + NOT_NULL + ", "
                    "TIMESTAMP " + get_sql_type<QDateTime>() + NOT_NULL
                    ")";
            try {
                dbWorker->run_query(request);

                std::thread checkCrackThread(&TransactionDatabaseManager::check_transaction_hashes, this);
                checkCrackThread.detach();
            } catch (Exceptions::Exception* exception) {
                exception->show();
            }

            break;
        }
        default:{
            exception->show();
        }
        }
    }
}

TransactionIdType TransactionDatabaseManager::add_transaction(UserIdType userId, PriceType price, TransactionType type)
{

    QString idRequest = "SELECT COALESCE(MAX(ID), 0) FROM " + dbName;

    try {
        QSqlQuery* idQuery = dbWorker->run_query(idRequest);

        if(idQuery->next()){
            TransactionIdType maxId = query_value_to_variable<TransactionIdType>(idQuery->value(0));

            QString hashRequest = "SELECT HASH FROM " + dbName + " WHERE (ID = " + QString::number(maxId) + ")";
            QSqlQuery* hashQuery = dbWorker->run_query(hashRequest);
            QString prevHash = "";
            if(hashQuery->next()){
                prevHash = hashQuery->value(0).toString();
            } else{
                //throw new Exceptions::DatabaseWorkingException(System::String("No previous hash in DB"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
            }

            QString currentString = prevHash + QString::number(userId) + "_" + QString::number(price) + "_" + QString::number(type) + localParameter;
            //std::cout << currentString.toStdString() << " - currentString" << std::endl;
            Transaction current = Transaction();

            current.id = maxId + 1;
            current.type = type;
            current.buyer = userId;
            current.price = price;
            current.timestamp = QDateTime::currentDateTimeUtc();
            current.hash = QString::fromStdString(System::SHA256().hash(currentString.toStdString()));

            return add_transaction(current);
        } else{
            QString currentString = QString::number(userId) + "_" + QString::number(price) + localParameter;

            Transaction current = Transaction();

            current.id = 1;
            current.type = type;
            current.buyer = userId;
            current.price = price;
            current.hash = QString::fromStdString(System::SHA256().hash(currentString.toStdString()));

            return add_transaction(current);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }


}

MafiaList<Transaction> TransactionDatabaseManager::get_all_transactions()
{
    QString request = "SELECT * FROM " + dbName;

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        MafiaList<Transaction> allTransactions = get_request_transactions(query);

        return allTransactions;
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<Transaction>();
        }
        }
    }

}

Transaction TransactionDatabaseManager::get_transaction(TransactionIdType id)
{

    QString request = "SELECT * FROM " + dbName + " WHERE (ID = " + QString::number(id) + ")";

    try {
        QSqlQuery* query = dbWorker->run_query(request);

        QSqlRecord record = query->record();
        if(query->next()){
            Transaction current = Transaction();

            current.id = query_value_to_variable<TransactionIdType>(query->value(record.indexOf("ID")));
            current.hash = query_value_to_variable<QString>(query->value(record.indexOf("HASH")));
            current.type = query_value_to_variable<TransactionType>(query->value(record.indexOf("TYPE")));
            current.buyer = query_value_to_variable<UserIdType>(query->value(record.indexOf("USER_ID")));
            current.price = query_value_to_variable<PriceType>(query->value(record.indexOf("PRICE")));

           return current;

        } else{
            throw new Exceptions::DatabaseWorkingException(System::String("Null database request answer"), Exceptions::DatabaseWorkingExceptionId_SQlQuery);
        }
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return Transaction();
        }
        }
    }

}

MafiaList<Transaction> TransactionDatabaseManager::get_users_transactions(UserIdType buyer)
{
    try {
        QSqlQuery* query = dbWorker->run_query("SELECT * FROM " + dbName + " WHERE (USER_ID = " + QString::number(buyer) + ")");
        MafiaList<Transaction> transactions = get_request_transactions(query);
        return transactions;

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<Transaction>();
        }
        }
    }
}

MafiaList<Transaction> TransactionDatabaseManager::get_time_bounded_transactions(QDateTime after, QDateTime before)
{
    try {
        QSqlQuery* query = dbWorker->run_query("SELECT * FROM " + dbName + " WHERE "
                                               "(TIMESTAMP >= " + after.toString(SQL_DATETIME_FORMAT) +
                                               " AND " + "TIMESTAMP <= " + before.toString(SQL_DATETIME_FORMAT) + ")");
        MafiaList<Transaction> transactions = get_request_transactions(query);
        return transactions;

    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return MafiaList<Transaction>();
        }
        }
    }
}

void TransactionDatabaseManager::check_database_cracked()
{
    while(dbWorker->database_open()){
        std::this_thread::sleep_for(std::chrono::milliseconds(hashCheckingInterval));
        try {
            bool databaseCracked = check_transaction_hashes();
            if(databaseCracked){
                on_database_cracked();
            }
        } catch (Exceptions::Exception* exception) {
            switch (exception->get_id()) {
            default:{
                exception->show();
            }
            }
        }

    }
}

bool TransactionDatabaseManager::check_transaction_hashes()
{
    MafiaList<Transaction> allTransactions = get_all_transactions();
    QString prevHash = "";
    for(int i = 0; i < allTransactions.length(); i++){
        QString currentString = prevHash + QString::number(allTransactions[i].buyer) + "_" +
                QString::number(allTransactions[i].price) + "_" + QString::number(allTransactions[i].type) +
                localParameter;
        QString expectedHash = QString::fromStdString(System::SHA256().hash(currentString.toStdString()));
        //std::cout << currentString.toStdString() << " - currentString" << std::endl;
        if(expectedHash != allTransactions[i].hash){
            std::cout << "Hash mismatch:\n" << expectedHash.toStdString() << "\n" << allTransactions[i].hash.toStdString() << "\n";
            return true;
        }
        prevHash = allTransactions[i].hash;
    }
    return false;
}

void TransactionDatabaseManager::on_database_cracked()
{
    std::cout << "DATABASE CRACKED!!!\n";
    dbWorker->restore_database();
}

TransactionIdType TransactionDatabaseManager::add_transaction(Transaction transaction)
{
    QString request = "INSERT INTO " + dbName + "(ID, USER_ID, PRICE, TYPE, HASH, TIMESTAMP)"
            "VALUES (%1, %2, %3, %4, %5, %6)";

    request = request.arg(transaction.id);
    request = request.arg(transaction.buyer);
    request = request.arg(transaction.price);
    request = request.arg(transaction.type);
    request = request.arg("\'" + transaction.hash + "\'");
    request = request.arg("\'" + transaction.timestamp.toString(SQL_DATETIME_FORMAT) + "\'");

    try {
        dbWorker->run_query(request);
        return transaction.id;
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        default:{
            throw exception;
            return -1;
        }
        }
    }
}

MafiaList<Transaction> TransactionDatabaseManager::get_request_transactions(QSqlQuery *query)
{
    MafiaList<Transaction> allTransactions = MafiaList<Transaction>();
    QSqlRecord record = query->record();
    while(query->next()){
        Transaction current = Transaction();

        current.id = query_value_to_variable<TransactionIdType>(query->value(record.indexOf("ID")));
        current.hash = query_value_to_variable<QString>(query->value(record.indexOf("HASH")));
        current.type = query_value_to_variable<TransactionType>(query->value(record.indexOf("TYPE")));
        current.buyer = query_value_to_variable<UserIdType>(query->value(record.indexOf("USER_ID")));
        current.price = query_value_to_variable<PriceType>(query->value(record.indexOf("PRICE")));
        current.timestamp = query_value_to_variable<QDateTime>(query->value(record.indexOf("TIMESTAMP")));

        allTransactions.append(current);

    }

    return allTransactions;
}
