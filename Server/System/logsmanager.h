#ifndef LOGSMANAGER_H
#define LOGSMANAGER_H
#include <QDateTime>
#include <fstream>

namespace Mafia {
	namespace System {
		//! \brief Тип лога (что передается этим логом).
		//! Используется в менеджере логов и передается в файл логов как int
		enum LogType{
			LogType_First = 0,
			LogType_Message = 0,
			LogType_Exception = 1,
			LogType_Crush = 2,
			LogType_Calls = 3,
			LogType_Last = 4
		};

		//! \brief Источник лога (откуда пришел лог).
		//! Логи разных источников сохраняются в разных файлах.
		enum LogSource {
			LogSource_First = 0,
			LogSource_MainServer = 0,
			LogSource_Subserver = 1,
			LogSource_Client = 2,
			LogSource_Last = 3
		};

		//! \brief Менеджер логов. По сути статический класс,
		//! который отвечает за все действия с логами.
		class LogsManager
		{
		public:
			/*!
			 * \brief Метод, который необходим для начала работы с логами.
			 * Вызовите его раньше всех остальных методов!
			 */
			static void setup();

			/*!
			 * \brief Метод добавления записи в файл с логами.
			 * \param message Сообщение, которое передается в этом логе (просто текст)
			 * \param logType Тип лога, который вы хотите назначить этой записи
			 * \param source Источник лога
			 * \param logSourceParams Некоторые параметры источника лога
			 * (например, для логов от клиентов это может быть ip и порт клиента, отправившего лог)
			 * \param timeStamp Время регистрации этого лога (оставьте пустым, чтобы назначить текущее время)
			 */
			static void add_record(std::string message,
								   LogType logType,
								   LogSource source,
								   std::string logSourceParams,
								   QDateTime timeStamp = QDateTime());

			/*!
			 * \brief Вторая версия метода добавления сообщения в файл логов.
			 * Использутеся, если у вас есть набор сообщений логов,
			 * которые вы хотите модифицировать.
			 * Например, используется при получении логов от пользователей
			 * \param data Все данные со всеми логами
			 * \param source Источник этих данных
			 * \param additionalSourceParams Дополнительные параметры источника, которые вы хотите добавить в лог
			 */
			static void add_record(std::string data,
								   LogSource source,
								   std::string additionalSourceParams);

		private:
			/*!
			 * \brief Пустой конструктор, который в общем-то не нужен. Т.к. класс по сути статический
			 */
			LogsManager();

			//! \brief Текстовый файл для записи в него логов основного сервера.
			static std::ofstream mainLogsFile;
			//! \brief Текстовый файл для записи в него логов клиентов
			static std::ofstream usersLogsFile;
			//! \brief Текстовый файл для записи в него логов субсерверов
			static std::ofstream subserversLogsFile;

			//! \brief Путь к папке, в которой хранятся логи
			static const std::string LOGS_PATH;

			//! \brief Формат времени, используемый для таймштампов в логах
			static const QString DATE_TIME_TYPE;

			//! \brief Размер текста (количество символов), выделяемый для таймштампа в логе
			static const int DATE_TIME_SIZE = 20;
			//! \brief Размер текста (количество символов), выделяемый для типа лога
			static const int LOG_TYPE_SIZE = 3;
			//! \brief Размер текста (количество символов), выделяемый для параметров источника лога
			static const int SOURCE_PARAMS_SIZE = 20;

			//! \brief Имя файла с основными логами
			static const std::string MAIN_LOGS_NAME;
			//! \brief Имя файла с логами пользователей
			static const std::string USERS_LOGS_NAME;
			//! \brief Имя файла с логами субсерверов
			static const std::string SUBSERVERS_LOGS_NAME;

			//! \brief Символ, которым будут заполнены пустоты в файле логов
			static const char EMPTY_CHARACTER = ' ';
		};
	}
}


#endif // LOGSMANAGER_H
