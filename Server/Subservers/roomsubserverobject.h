#ifndef ROOMSUBSERVEROBJECT_H
#define ROOMSUBSERVEROBJECT_H
#include "subserverobject.h"
#include <MafiaRequests>
#include <MafiaGameplay>
#include "userstatistics.h"
#include "clientinfo.h"

namespace Mafia {
    namespace Subservers {
        //! \brief Класс для API субсервера-комнаты
        class RoomSubserverObject : public SubserverObject
        {
        Q_OBJECT
        public:
            /*!
             * \brief В конец задолбавший меня конструктор, наследуемый от QObject
             * \param parent Фигня, которая нужна Qt
             */
            explicit RoomSubserverObject(QObject* parent = nullptr);

            /*!
             * \brief Основной конструктор, который следует использовать
             * \param networker Указатель на объект для работы с сетью
             * \param port Порт, на котором следует инициализировать субсервер
             * \param checkInterval Временной интервал (мс) через который проводится проверка соединения с субсервером
             * (если не задавать значение, то будет равен 2000 мс)
             * \param maxNotAnswering Максимальное количество 'не ответов' на запрос подтверждения активности субсервера,
             * после которого будет считаться, что субсервер упал и он будет перезапущен (если не задать, то будет равно 5)
             * \param specialCommands Дополнительные команды, которые следует передать при запуске программы
             */
			RoomSubserverObject(Network::Networker* networker, int port, int checkInterval = 2000, int maxNotAnswering = 5,
                                const System::String specialCommands = System::String(""));

            //! \brief Деструктор (пока пустой)
            ~RoomSubserverObject();

            /*!
              * \brief Функция передачи клиента субсерверу
              * \param client Клиент, которого следует передать субсерверу
              * \param info Информация о пользователе, под которым зарегистрирован субсервер
              */
             void pass_client(Network::Client client, UserStatistics info);

             /*!
              * \brief Получение списка всех клиентов, которые подключены в данный момент к субсерверу
              * \return Список информации обо всех клиентах, в данный момент подключенных к субсерверу
              */
             MafiaList<ClientInfo> get_connected_clients();

             /*!
              * \brief Функция проверки совпадения ключа подтверждения
              * \param key Ключ, который ввел пользователь
              * \return true, если ключ подходит, иначе - false
              */
             bool check_connection_key(QString key);

             /*!
              * \brief Функция получения ключа подтверждения
              * \return Строку, содержащую ключ подтверждения
              */
             QString get_key();

        signals:
             /*!
              * \brief Сигнал, вызываемый при завершении игры
              * \param gameData Вся информация о прошедшей игре
              * \param me Указатель на объект субсервера для его идентификации
              */
             void game_ended(Gameplay::Game gameData, RoomSubserverObject* me);

             /*!
              * \brief Сигнал, вызываемый при покидании каким-либо клиентом субсервера
              * \param client Клиент, который покинул субсервер
              */
             void client_leave(Network::Client client);

        private:
             /*!
              * \brief Функция обработки пришедшего сообщения от субсервера
              * \param message Все данные сообщения
              */
			 void message_from_server(Network::Message_t message) override;

             /*!
              * \brief Список клиентов и данных о них, которые в данный момент подключены к этому субсерверу
              */
             MafiaList<ClientInfo> myClients;

             //! \brief Ключ для входа в игру
             const QString connection_key;

             //! Путь к исполняемому файлу субсервера
             static const System::String exePath;

             //! Имя процесса субсервера
             static const System::String exeProcessName;
             //! \brief Размер строки ключа для входа в игру
             static const int KEY_SIZE = 20;
        };
    }
}


#endif // ROOMSUBSERVEROBJECT_H
