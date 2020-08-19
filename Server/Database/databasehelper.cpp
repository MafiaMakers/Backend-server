#include "databasehelper.h"
#include "Gameplay/gameitems.h"
#include "chatmessage.h"
#include "chatsettingsdatabasemanager.h"

using namespace Mafia;
using namespace Database;

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
	QDateTime timestamp = data.toDateTime();
	timestamp.setTimeSpec(Qt::UTC);
	return timestamp;
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

template<>
QString Mafia::Database::get_sql_type<int>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<QString>(){
    return "VARCHAR(" + QString::number(STRING_SIZE) + ")";
}

template<>
QString Mafia::Database::get_sql_type<bool>(){
    return "BIT";
}

template<>
QString Mafia::Database::get_sql_type<MafiaList<int>>(int maxListCount){
    return "VARCHAR(" + QString::number(maxListCount * sizeof(int) * 2)+ ")";
}

template<>
QString Mafia::Database::get_sql_type<QDateTime>(){
    return "DATETIME";
}

template<>
QString Mafia::Database::get_sql_type<Status>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<Achievement>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<AccountType>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<Gameplay::Role>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<Gameplay::GameResult>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<MafiaList<Gameplay::Role>>(int maxListCount){
    return "VARCHAR(" + QString::number(maxListCount * sizeof(int) * 2)+ ")";
}

template<>
QString Mafia::Database::get_sql_type<MafiaList<ChatCapability>>(int maxListCount){
    return "VARCHAR(" + QString::number(maxListCount * sizeof(int) * 2)+ ")";
}

template<>
QString Mafia::Database::get_sql_type<ChatCapability>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<TransactionType>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<AuthorizedStatus>(){
    return "integer";
}

template<>
QString Mafia::Database::get_sql_type<ChatFeature>(){
    return "integer";
}


QString Mafia::Database::get_sql_filter(FilterType filter)
{
    switch (filter) {
		case FilterType_OR:{
			return " OR ";
		}
		case FilterType_AND:{
			return " AND ";
		}
		case FilterType_NONE:{
			throw Exceptions::Exception::generate(System::String("Incorrect filter!"),
														   Exceptions::DatabaseWorkingExceptionId_UnknownFilterType);
			break;
		}
    }
    return "";
}

DATE_TIME Mafia::Database::from_string(STRING data)
{
#ifndef DONT_USE_QT
	QDateTime result = QDateTime::fromString(data, SQL_DATETIME_FORMAT);
	result.setTimeSpec(Qt::UTC);
	return result;
#else
	return data;
#endif
}
