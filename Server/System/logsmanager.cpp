#include "logsmanager.h"
#include "Exceptions/systemexception.h"

using namespace Mafia;
using namespace System;

const std::string LogsManager::LOGS_PATH = "Logs";
const std::string LogsManager::MAIN_LOGS_NAME = "Main.log";
const std::string LogsManager::USERS_LOGS_NAME = "Users.log";
const std::string LogsManager::SUBSERVERS_LOGS_NAME = "Subservers.log";

std::ofstream LogsManager::mainLogsFile;
std::ofstream LogsManager::subserversLogsFile;
std::ofstream LogsManager::usersLogsFile;

const QString LogsManager::DATE_TIME_TYPE = "yyyy.MM.dd hh:mm:ss";

void LogsManager::setup()
{
	//Просто открываем все файлы

	mainLogsFile = std::ofstream();
	mainLogsFile.open(LOGS_PATH + "\\" + MAIN_LOGS_NAME, std::ios_base::app);

	usersLogsFile = std::ofstream();
	usersLogsFile.open(LOGS_PATH + "\\" + USERS_LOGS_NAME, std::ios_base::app);

	subserversLogsFile = std::ofstream();
	subserversLogsFile.open(LOGS_PATH + "\\" + SUBSERVERS_LOGS_NAME, std::ios_base::app);
}

void LogsManager::add_record(std::string message,
							 LogType logType,
							 LogSource source,
							 std::string sourceParams,
							 QDateTime timestamp)
{
	//Если время не указано, берем текущее
	if(timestamp.isNull()){
		timestamp = QDateTime::currentDateTimeUtc();
	}

	std::string resultMessage = "";

	//Сначала добавляем данные о времени
	std::string timestampStr = timestamp.toString(DATE_TIME_TYPE).toStdString();
	for (int timestampIndex = 0; timestampIndex < DATE_TIME_SIZE; timestampIndex++){
		if((unsigned int)timestampIndex < timestampStr.length()){
			resultMessage += timestampStr[ timestampIndex ];
		} else{
			resultMessage += EMPTY_CHARACTER;
		}

	}

	resultMessage += sourceParams;

	//Затем данные о источнике логов
	for(int paramsIndex = 0; paramsIndex < SOURCE_PARAMS_SIZE; paramsIndex++) {
		if((unsigned int)paramsIndex < sourceParams.length()){
			resultMessage += sourceParams[ paramsIndex ];
		} else{
			resultMessage += EMPTY_CHARACTER;
		}

	}

	//Затем тип лога
	std::string logTypeStr = QString::number((int)logType).toStdString();
	for(int logTypeIndex = 0; logTypeIndex < LOG_TYPE_SIZE; logTypeIndex++) {
		if((unsigned int)logTypeIndex < logTypeStr.length()){
			resultMessage += logTypeStr[ logTypeIndex ];
		} else{
			resultMessage += EMPTY_CHARACTER;
		}
	}

	//И, наконец, само сообщение лога
	resultMessage += message;

	//В зависимости от типа лога, он отправляется в разные файлы
	switch (source) {
		case LogSource_MainServer:{
			mainLogsFile << resultMessage << std::endl;
			break;
		}
		case LogSource_Subserver:{
			subserversLogsFile << resultMessage << std::endl;
			break;
		}
		case LogSource_Client:{
			usersLogsFile << resultMessage << std::endl;
			break;
		}
		default:{
			throw Exceptions::Exception::generate(String("Invalid log source type"), Exceptions::SystemExceptionId_LogsError);
			break;
		}
	}
}

void LogsManager::add_record(std::string data, LogSource source, std::string additionalSourceParams)
{
	unsigned int firstIndex = 0;
	//Разбиваем лог по строкам
	for(unsigned int dataIndex = 0; dataIndex < data.length(); dataIndex++){
		if(data[dataIndex] == '\n'){
			//Выделяем отдельную строку
			std::string line = data.substr(firstIndex, dataIndex);
			int index = 0;

			//Далее выделяем строку, содержащую время
			std::string dateTimeStr = line.substr(index, DATE_TIME_SIZE);
			index += DATE_TIME_SIZE;

			//Содержащую информацию об источнике данных
			std::string sourceParams = line.substr(index, index + SOURCE_PARAMS_SIZE);
			index += SOURCE_PARAMS_SIZE;

			//Тип лога
			std::string logType = line.substr(index, index + LOG_TYPE_SIZE);
			index += LOG_TYPE_SIZE;

			//И само сообщение
			std::string message = line.substr(index, line.length());

			//Превращаем обратно в дату, тип лога и строку. Изменяем параметры источника
			QDateTime dateTime = QDateTime::fromString(QString::fromStdString(dateTimeStr), DATE_TIME_TYPE);
			LogType type = (LogType)atoi(logType.c_str());
			sourceParams += additionalSourceParams;

			add_record(message, type, source, sourceParams, dateTime);

			//Переходим к следующей строке
			firstIndex = dataIndex + 1;
		}
	}
}

LogsManager::LogsManager(){}
