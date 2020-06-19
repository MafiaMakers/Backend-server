#include "databasehelper.h"
#include "Gameplay/gameitems.h"
#include "chatmessage.h"
#include "chatsettingsdatabasemanager.h"

using namespace Mafia;
using namespace Database;

QString Mafia::Database::generate_salt()
{
    const int saltSize = 10;

    QString salt = "";

    for(int i = 0; i < saltSize; i++){
        salt += (char)((char)'A' + (char)(qrand() % 26));
    }

    return salt;
}





template<>
int Mafia::Database::query_value_to_variable(QVariant data)
{
    return data.toInt();
}

template<>
QString Mafia::Database::query_value_to_variable(QVariant data)
{
    return data.toString();
}

template<>
QDateTime Mafia::Database::query_value_to_variable(QVariant data)
{
    return data.toDateTime();
}

template<>
MafiaList<int> Mafia::Database::query_value_to_variable(QVariant data)
{
    return qlist_from_byte_array<int>(data.toByteArray());
}

template<>
MafiaList<Gameplay::Role> Mafia::Database::query_value_to_variable(QVariant data)
{
    return qlist_from_byte_array<Gameplay::Role>(data.toByteArray());
}

template<>
Status Mafia::Database::query_value_to_variable(QVariant data)
{
    return (Status)data.toInt();
}

template<>
bool Mafia::Database::query_value_to_variable(QVariant data)
{
    return data.toBool();
}


template<>
ChatFeature Mafia::Database::query_value_to_variable(QVariant data)
{
    return (ChatFeature)data.toInt();
}

template<>
Achievement Mafia::Database::query_value_to_variable(QVariant data)
{
    return (Achievement)data.toInt();
}

template<>
AuthorizedStatus Mafia::Database::query_value_to_variable(QVariant data)
{
    return (AuthorizedStatus)data.toInt();
}

template<>
AccountType Mafia::Database::query_value_to_variable(QVariant data)
{
    return (AccountType)data.toInt();
}

template<>
TransactionType Mafia::Database::query_value_to_variable(QVariant data)
{
    return (TransactionType)data.toInt();
}

template<>
Gameplay::Role Mafia::Database::query_value_to_variable(QVariant data)
{
    return (Gameplay::Role)data.toInt();
}

template<>
Gameplay::GameResult Mafia::Database::query_value_to_variable(QVariant data)
{
    return (Gameplay::GameResult)data.toInt();
}

template<>
ChatCapability Mafia::Database::query_value_to_variable(QVariant data)
{
    return (ChatCapability)data.toInt();
}

template<>
MafiaList<ChatCapability> Mafia::Database::query_value_to_variable(QVariant data)
{
    return qlist_from_byte_array<ChatCapability>(data.toByteArray());
}
