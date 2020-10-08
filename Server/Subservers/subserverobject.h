#ifndef SUBSERVEROBJECT_H
#define SUBSERVEROBJECT_H

#include <QObject>
#include "Network/message.h"
#include "Network/messageTypes.h"
#include "Network/networker.h"
#include "System/simplestring.h"
#include "Exceptions/exception.h"
#include "Requests/networkrequest.h"

//! Тип, в котором хранятся id запросов
typedef int RequestIdType;

namespace Mafia {
    //! \brief Пространство имен для работы с API субсерверов
    namespace Subservers {
        //! Структура для хранения сетевого запроса и его id
        struct RequestWithId{
            //! Указатель на сетевой запрос
            Requests::NetworkRequest* request;
            //! id этого запроса
            RequestIdType id;
        };

        /*!
         * \brief Базовый класс субсервера. Он запускает процесс субсервера, отслеживает краши этого процесса,
         * может отправлять ему запросы и получать сообщения и ответы на запросы.
         */
        class SubserverObject : public QObject
        {
            Q_OBJECT
        public:
            /*!
             * \brief Конструктор, наследуемый от QObject'a
             * \param parent какая-то Qt-шная фигня...
             */
            explicit SubserverObject(QObject *parent = nullptr);
            /*!
             * \brief Основной конструктор, который стоит использовать
             * \param networker Указатель на объект MainServerNetworker, который отправляет сообщения
             * \param port Порт, на котором стоит запустить субсервер
             * \param path Путь к исполняемому файлу субсервера (включая имя исполняемого файла)
             * \param processName Имя процесса, который будет запущен при вызове исполняемого файла
             * \param checkInterval Временной интервал (мс) между проверками того, что субсервер отвечает (стандартное значение 2000)
             * \param maxNotAnswering Максимальное количество пропущенных запросов на подтверждение активности субсервера (стандартное значение 5)
             * \param specialCommands Специальные параметры, которые дополнительно надо передать при запуске субсервера (стандартное значение "")
             */
			SubserverObject(Network::Networker* networker, int port, const System::String path, const System::String processName,
                            int checkInterval = 2000, int maxNotAnswering = 5, const System::String specialCommands = System::String(""));

            //! \brief Деструктор (пока пустой)
            ~SubserverObject();
            /*!
             * \brief Метод отправления запроса субсерверу
             * \param type Тип сообщения, которое надо отправить субсерверу
             * \param data Данные, которые надо передать в сообщении
             * \param size Размер массива данных
             * \return id реквеста, по которому потом можно обратиться в этот же объект SubserverObject и получить сведения
             */
			//virtual RequestIdType send_request(Network::MessageType type, Network::SymbolType* data, int size);

            /*!
             * \brief Функция получения результата запроса по id запроса
             * \param requestId id запроса, у которого надо получить данные
             */
			//template<class T>
			//T get_request_result(RequestIdType requestId);
            /*!
             * \brief Функция проверки, готов ли ответ на указанный запрос
             * \param requestId id запроса, который следует проверить на готовность
             * \return true, если запрос готов вернуть данные, иначе false
             */
			//bool is_request_ready(RequestIdType requestId);
            /*!
             * \brief Функция отправки сообщения субсерверу
             * \param type Тип сообщения
             * \param data массив данных сообщения
             * \param size размер массива данных
             * \param id id сообщения (стандартное значение 0)
             * \return id отправленного сообщения
             */
			int send_message_to_subserver(Network::MessageType type, Network::SymbolType* data, int size, Network::MessageIdType id = 0);

            /*!
             * \brief Функция получения адреса субсервера
             * \return Адрес этого субсервера
             */
            Network::Client get_my_address();

            /*!
             * \brief Функция принудительного завершения процесса субсервера
             */
            void finish_work();
        signals:
            /*!
             * \brief Сигнал, вызываемый при готовности запроса
             * \param requestId id запроса, который стал готов к отдаче информации
             */
			//void on_request_ready(RequestIdType requestId);
            /*!
             * \brief Сигнал, вызываемый при краше субсервера (перезапуск производится автоматически)
             */
            void on_crash();
            /*!
             * \brief Сигнал, вызываемый при получении сообщения от субсервера
             * \param message сообщение, которое получено от субсервера
             */
			void on_message_got(Network::Message_t message);
        protected slots:
            /*!
             * \brief Слот, который привязывается к запросу и вызывает сигнал \ref Mafia::SubserverObject::on_request_ready(RequestIdType requestId)
             * \param request указатель на запрос, который стал готов к отдаче информации
             */
			//virtual void request_answered(Requests::Request* request);

            /*!
             * \brief Сигнал, вызываемый при получении сообщения от локалхоста.
             * Проверяет от этого ли субсервера пришло сообщение и если от этого, то вызывает сигнал
             * \ref Mafia::SubserverObject::on_message_got(Mafia::Message message)
             * \param message Сообщение, полученное от сервера
             */
			virtual void message_from_server(Network::Message_t message);

            /*!
             * \brief Слот, обрабатывающий краш субсервера (в т.ч. перезапускает его)
             */
            virtual void crash_processing();
        protected:
            /*!
             * \brief Метод, который все время существования субсервера проверяет, активен ли сейчас субсервер. И если субсервер не активен,
             * то перезапускает его
             */
            virtual void _check_subserver_respond();

            /*!
             * \brief Функция запуска субсервера с указанием дополнительных комманд
             * \param specialCommands дополнительные параметры, которые надо передать субсерверу для запуска
             */
            virtual void run_me(const System::String specialCommands);

            //! Адрес субсервера (127.0.0.1 и порт)
            Network::Client myAddress;

            //! Список всех запросов этого субсервера
			//MafiaList<RequestWithId> currentRequests;

            //! Указатель на объект \ref Mafia::MainServerNetworker , используемый для создания запросов и отправления сообщений
			Network::Networker* networker;

            //! PID процесса субсервера
            int pid;

            //! булевая переменная, отвечающая за то, работает ли еще субсервер
            bool initialized = false;

            //! Текущее количество не отвеченных подряд запросов на подтверждение активности субсервера
            //! (если оно превысит \ref Mafia::SubserverObject::maxNotAnsweringsCount , то будет считаться, что субсервер упал и будет вызван сигнал
            //! \ref Mafia::SubserverObject::on_crash())
            int notAnsweringsCount = 0;

            //! Путь к исполняемому файлу субсервера
            const System::String path;

            //! Имя процесса субсервера
            const System::String processName;

            //! Временной интервал (мс) между проверками активности субсервера
            const int checkRespondInterval = 2000;

            //! Максимальное допустимое количество раз, сколько субсервер может не ответить на запрос.
            //! Если он не ответит большее число раз, то будет считаться, что он упал и будет вызван сигнал
            //! \ref Mafia::SubserverObject::on_crash())
            const int maxNotAnsweringsCount = 5;
        };
    }

}


#endif // SUBSERVEROBJECT_H
