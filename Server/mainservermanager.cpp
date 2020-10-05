#include "mainservermanager.h"
#include "userstatistics.h"
using namespace Mafia;

const MafiaList<QString> MainServerManager::errorEmailReceivers =
		MafiaList<QString>() << "andrusov,n@gmail.com";

const Database::UserIdType MainServerManager::nullUser = -1;


MainServerManager::MainServerManager(int argc, char* argv[], QObject* parent) : QObject(parent)
{
    for(int i = 1; i < argc; i++){
        std::cout << argv[i] << std::endl;
    }

    try {
		System::LogsManager::setup();

		Network::Crypto::set_key("cdbhjDH%R^%^&@bhfcdsjVg");

		networker = new Network::MainServerNetworker(System::PortsManager::allocate_port());

		backupSubserver = new Subservers::BackupSubserverObject(networker, System::PortsManager::allocate_port());

		games = MafiaList<Subservers::RoomSubserverObject*>();

		dbWorker = new Database::DatabaseWorker();

		usersDb = new Database::UserDatabaseManager(dbWorker);

		gamesDb = new Database::GameDatabaseManager(dbWorker);

		chatSettingsDb = new Database::ChatSettingsDatabaseManager(dbWorker);

		chatsDb = new Database::ChatDatabaseManager(dbWorker);

		transactionsDb = new Database::TransactionDatabaseManager(dbWorker);

		connect(chatsDb, &Database::ChatDatabaseManager::on_message_read, this, &MainServerManager::on_chat_message_read);
		connect(chatsDb, &Database::ChatDatabaseManager::on_message_sent, this, &MainServerManager::on_chat_message_sent);
		connect(chatsDb, &Database::ChatDatabaseManager::on_message_edited, this, &MainServerManager::on_chat_message_edited);
		connect(chatsDb, &Database::ChatDatabaseManager::on_message_deleted, this, &MainServerManager::on_chat_message_deleted);

		connect_to_processor();

		if(!dbWorker->database_ready()){
			solve_database_error();
		}

    } catch (Exceptions::Exception* exception) {
        exception->show();
    }

	std::cout << "Main server started\n";

	System::LogsManager::add_record("Main server started",
									System::LogType_Message,
									System::LogSource_MainServer,
									"");

}

void MainServerManager::on_chat_message_sent(Database::ChatMessage message)
{
    try {
		//Получаем список пользователей, которых следует уведомить об отправке сообщения
        MafiaList<Database::UserIdType> ids = chatSettingsDb->get_chat(message.toChat).users;

        std::string messageData = System::Serializer::serialize<Database::ChatMessage>(message);

		//Выбираем тех пользователей, которые онлайн и отправляем им сообщения
        for(int i = 0; i < ids.length(); i++){
            if(users.contains(ids[i])){
                Network::Client client = get_client_by_user(ids[i]);

				networker->send_message(Network::Message(
											Network::MessageType_NewChatMessage,
											(Network::SymbolType*)messageData.c_str(),
											messageData.length(),
											client)
										);
            }
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing( exception );
    }
}

void MainServerManager::on_chat_message_edited(Database::ChatMessage message)
{
    try {
		//Получаем список пользователей, которых следует уведомить об изменении сообщения
        MafiaList<Database::UserIdType> ids = chatSettingsDb->get_chat(message.toChat).users;

        std::string messageData = System::Serializer::serialize<Database::ChatMessage>(message);

		//Выбираем тех, кто онлайн и отправляем им сообщения
        for(int i = 0; i < ids.length(); i++){
            if(users.contains(ids[i])){
                Network::Client client = get_client_by_user(ids[i]);

				networker->send_message(Network::Message(
											Network::MessageType_EditedChatMessage,
											(Network::SymbolType*)messageData.c_str(),
											messageData.length(),
											client)
										);
            }
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing( exception );
    }
}

void MainServerManager::on_chat_message_read(Database::MessageIdType message, Database::ChatIdType chat, Database::UserIdType user)
{
    try {
		//Получаем список пользователей, которых следует уведомить о прочтении сообщения
        MafiaList<Database::UserIdType> ids = chatSettingsDb->get_chat(chat).users;

        std::string messageData = System::Serializer::serialize<System::Tuple<Database::MessageIdType, Database::UserIdType>>
                (System::Tuple<Database::MessageIdType, Database::UserIdType>(message, user));

		//Выбираем тех, кто онлайн и отправляем им сообщение
        for(int i = 0; i < ids.length(); i++){
            if(users.contains(ids[i])){
                Network::Client client = get_client_by_user(ids[i]);

				networker->send_message(Network::Message(
											Network::MessageType_OnReadChatMessage,
											(Network::SymbolType*)messageData.c_str(),
											messageData.length(),
											client)
										);
            }
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing( exception );
		exception->close();
    }
}

void MainServerManager::on_chat_message_deleted(Database::MessageIdType messageId, Database::ChatIdType chat)
{
    try {
		//Получаем список пользователей, которых следует уведомить об удалении сообщения
        MafiaList<Database::UserIdType> ids = chatSettingsDb->get_chat(chat).users;

        std::string messageData = System::Serializer::serialize<Database::MessageIdType>(messageId);

		//Выбираем тех, кто онлайн и отправляем им сообщение
        for(int i = 0; i < ids.length(); i++){
            if(users.contains(ids[i])){
                Network::Client client = get_client_by_user(ids[i]);

				networker->send_message(Network::Message(
											Network::MessageType_DeletedChatMessage,
											(Network::SymbolType*)messageData.c_str(),
											messageData.length(),
											client)
										);
            }
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		exception->close();
    }
}

void MainServerManager::create_user(QString nickname, QString email, QString password, Network::Client client,
                                    Network::MessageIdType requestId)
{
    try {
		//Добавляем пользователя в БД и получаем его ключ подтверждения почты
        QString confirmationKey = "";
        Database::UserIdType newUserId = usersDb->add_user(nickname, email, password, confirmationKey);

		//Если до этого он уже был авторизован, то логаутимся и входим под новой записью
        if(clients.contains(client)){
            int index = clients.indexOf(client);
            if(users[index] != nullUser){
                usersDb->logout_user(users[index]);
				networker->send_message(Network::Message(
											Network::MessageType_LoggedOut,
											(Network::SymbolType*)"",
											1,
											client)
										);
            }
            users[index] = newUserId;
		//Иначе просто добавляем нового клиента и нового пользователя уже авторизованным
        } else{
            clients.append(client);
            users.append(newUserId);
        }

		//Отправляем ему сообщение о том, что он успешно зарегистрировался и вошел
        System::Tuple<Database::UserIdType, QString> messageData = System::Tuple<Database::UserIdType, QString>(newUserId, email);

        std::string messageText = System::Serializer::serialize<System::Tuple<Database::UserIdType, QString>>(messageData);

		networker->send_message(Network::Message(
									Network::MessageType_RequestAnswer,
									(Network::SymbolType*)messageText.c_str(),
									messageText.length(),
									client,
									requestId)
								);

		//Создаем и отправляем email для подтверждения ему на почту
        QString mailMessageText = "You received this message because your email was connected with " + nickname + "'s account "
				" in Mafia online game. This is your confirmation key: " + confirmationKey + ". Put it in the respective field in "
                "game and your registration will be completed.";

        System::MailSender::send_email(email, "Welcome to Mafia Online game", mailMessageText);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		if(exception->get_id() == Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult){
			networker->send_message(Network::Message(
										Network::MessageType_RequestAnswer,
										(Network::SymbolType*)"",
										0,
										client,
										requestId)
									);
		}
		exception->close();
    }
}

void MainServerManager::send_chat_message(Network::Client sender, Database::ChatIdType toChat, QString data,
										  MafiaList<Database::MessageIdType> answerFor, Database::ChatFeature feature)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType senderUser = get_user_by_client(sender);
		check_null_user(senderUser);
		//Собираем сообщение в чат
        Database::ChatMessage message = Database::ChatMessage();
        message.data = data;
        message.from = senderUser;
        message.toChat = toChat;
        message.feature = feature;
        message.answerFor = answerFor;

		//Проверяем, есть ли у этого пользователя права на отправку сообщений в этот чат
		//Если все ок, то отправляем сообщение в чат.
		//Иначе отправляем пользователю сообщение о том, что у него нет прав
        if(chatSettingsDb->can_send_message(senderUser, toChat)){
            chatsDb->add_message(message);
        } else{
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"",
										1,
										sender)
									);
        }

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, sender);
		exception->close();
    }
}

void MainServerManager::login_user(QString email, QString password, Network::Client client, Network::MessageIdType requestId)
{
    try {
        Database::UserIdType userId = usersDb->get_id(email);
        bool login = usersDb->login_user(userId, password);
		//Проверяем, удалось ли войти
        if(login){
			//Если клиент уже был под какой-то учетной записью,
			//то логаутимся и входим под новым аккаунтом
			if(clients.contains( client )){
				int clientIndex = clients.indexOf( client );

				usersDb->logout_user( users[ clientIndex ] );
				users[ clientIndex ] = userId;
			//Иначе просто добавляем нового клиента
            } else{
                clients.append(client);
                users.append(userId);
            }

			//Отправляем сообщение об успешном входе
            System::Tuple<Database::UserIdType, QString> messageData = System::Tuple<Database::UserIdType, QString>(userId, email);

            std::string messageText = System::Serializer::serialize<System::Tuple<Database::UserIdType, QString>>(messageData);

			networker->send_message(Network::Message(
										Network::MessageType_RequestAnswer,
										(Network::SymbolType*)messageText.c_str(),
										messageText.length(),
										client,
										requestId)
									);
        } else{
			//Отправляем ответ на запрос, но пустой, т.к. не удалось войти
            networker->send_message(Network::Message(Network::MessageType_RequestAnswer,
                                                     (Network::SymbolType*)"", 0, client, requestId));
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::create_chat(Network::Client creator, Network::MessageIdType requestId)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType creatorUser = get_user_by_client(creator);
		check_null_user(creatorUser);

		//Создаем новый чат
        Database::ChatIdType newId = chatSettingsDb->create_chat();
		//Добавляем в него создателя
        chatSettingsDb->add_user_to_chat(creatorUser, newId, Database::ChatCapabilities_Admin);

		//Отправляем ему сообщение об успешном создании чата
        std::string messageData = System::Serializer::serialize<Database::ChatIdType>(newId);

		networker->send_message(Network::Message(
									Network::MessageType_RequestAnswer,
									(Network::SymbolType*)messageData.c_str(),
									messageData.length(),
									creator,
									requestId)
								);
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, creator);
		exception->close();
    }

}

void MainServerManager::get_last_messages(Network::Client client,
										  Database::ChatIdType chat,
										  Network::MessageIdType requestId,
										  int messagesCount)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);

		//Проверяем, что этот пользователь может читать сообщения из этого чата
        bool canGet = chatSettingsDb->can_read_message(user, chat);

        if(canGet){
			//Получаем сообщения из БД
            MafiaList<Database::ChatMessage> allMessages;
            if(messagesCount == -1){
                allMessages = chatsDb->get_last_messages(chat);
            } else{
                allMessages = chatsDb->get_last_messages(chat, messagesCount);
            }

			//Отправляем пользователю эти сообщения
			std::string data = System::Serializer::serialize<MafiaList<Database::ChatMessage>>( allMessages );

			networker->send_message(Network::Message(
										Network::MessageType_RequestAnswer,
										(Network::SymbolType*)data.c_str(),
										data.length(),
										client,
										requestId)
									);
        } else{
			//Отправляем пустой ответ на запрос
			networker->send_message(Network::Message(
										Network::MessageType_RequestAnswer,
										(Network::SymbolType*)"",
										0,
										client,
										requestId)
									);
			//Отправляем пользователю информацию о том, что он не может читать сообщения из этого чата
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't read messages from this chat",
										39,
										client)
									);
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::get_users_chats(Network::Client client, Network::MessageIdType requestId, int chatsCount)
{
	try {
		//Проверяем, что пользователь авторизован
		Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);

		//Получаем список всех чатов этого пользователя
		MafiaList<Database::ChatIdType> usersChats = usersDb->get_user(user).chats;

		auto messages = MafiaList<Database::ChatMessage>();

		//Из каждого чата получаем последнее сообщение
		for(int chatIndex = 0; chatIndex < usersChats.length(); chatIndex++){
			MafiaList<Database::ChatMessage> currentChatMessages = chatsDb->get_last_messages(usersChats[ chatIndex ], 1);
			if(currentChatMessages.length() == 1){
				messages.append(currentChatMessages[0]);
			} else{
				auto messageToAdd = Database::ChatMessage();
				messageToAdd.toChat = usersChats[ chatIndex ];
				messageToAdd.timestamp = QDateTime::currentDateTimeUtc();
			}
		}

		//Сортируем сообщения по дате
		for(int messageIndex = 0; messageIndex < messages.length(); messageIndex++){
			for(int currentMessageIndex = 1; currentMessageIndex < messages.length() - messageIndex; currentMessageIndex++){
				if(messages[ currentMessageIndex ].timestamp < messages[ currentMessageIndex - 1 ].timestamp){
					auto tmp = messages[ currentMessageIndex - 1 ];
					messages[ currentMessageIndex - 1 ] = messages[ currentMessageIndex ];
					messages[ currentMessageIndex ] = tmp;
				}
			}
		}

		auto chats = MafiaList<Database::ChatIdType>();

		//Выбираем чаты, в которых самые последние сообщения
		for(int messageIndex = 0; messageIndex < messages.length() && messageIndex < chatsCount; messageIndex++){
			chats.append( messages[ messageIndex ].toChat );
		}

		//Отправляем пользователю сообщение со списком чатов, отсортированных по дате последнего сообщения
		std::string data = System::Serializer::serialize<MafiaList<Database::ChatIdType>>(chats);

		networker->send_message(Network::Message(
									Network::MessageType_RequestAnswer,
									(Network::SymbolType*)data.c_str(),
									data.length(),
									client,
									requestId)
								);

	} catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
	}
}

void MainServerManager::add_user_to_chat(Database::ChatIdType chat,
										 Database::UserIdType user,
										 Network::Client initializer,
                                         Database::ChatCapability capability)
{
    try{
		//Проверяем, что пользователь авторизован
		Database::UserIdType initializerUser = get_user_by_client(initializer);
		check_null_user(initializerUser);

		//Проверяем, что этот пользователь может добавлять и удалять в чат пользователей
        bool ableToEdit = chatSettingsDb->can_edit_users(initializerUser, chat);
        if(ableToEdit){
			//Если все ок, то добавляем
            chatSettingsDb->add_user_to_chat(user, chat, capability);
        } else{
			//Если не ок, то отправляем сообщение о том, что пользователь не имеет права на такое действие
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't edit users in this chat",
										33,
										initializer)
									);
        }
    }
    catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, initializer);
		if(exception->get_id() == Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt){
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"User is already in chat",
										23,
										initializer)
									);
        }
		exception->close();
    }
}

void MainServerManager::remove_user_from_chat(Database::ChatIdType chat, Database::UserIdType user, Network::Client initializer)
{

    try{
		//Проверяем, что пользователь авторизован
		Database::UserIdType initializerUser = get_user_by_client(initializer);
		check_null_user(initializerUser);

		//Проверяем, что он может менять пользователей этого чата
        bool ableToEdit = chatSettingsDb->can_edit_users(initializerUser, chat);
        if(ableToEdit){
            chatSettingsDb->remove_user_from_chat(user, chat);
        } else{
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't edit users in this chat",
										33,
										initializer)
									);
        }
    }
    catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, initializer);
		if(exception->get_id() == Exceptions::DatabaseWorkingExceptionId_DeleteMissedItemAttempt){
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"User is not in chat",
										23,
										initializer)
									);
        }
		exception->close();

    }
}

void MainServerManager::change_users_chat_capability(Database::ChatIdType chat,
													 Database::UserIdType user,
													 Database::ChatCapability newCapability,
                                                     Network::Client initializer)
{
    try{
		//Проверяем, что пользователь авторизован
		Database::UserIdType initializerUser = get_user_by_client(initializer);
		check_null_user(initializerUser);

		//Проверяем, что этот пользователь может менять пользователей этого чата
        bool ableToEdit = chatSettingsDb->can_edit_users(initializerUser, chat);
        if(ableToEdit){
            chatSettingsDb->set_capability(user, chat, newCapability);
        } else{
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't edit users in this chat",
										33,
										initializer)
									);
        }
    }
    catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, initializer);
		if(exception->get_id() == Exceptions::DatabaseWorkingExceptionId_DoubleAddingItemAttempt){
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"User is already in chat",
										23,
										initializer)
									);
        }
		exception->close();
    }
}

void MainServerManager::create_game(Network::Client creator)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(creator);
		check_null_user(user);

		//Запускаем новый субсервер-комнату
		Subservers::RoomSubserverObject *current =
				new Subservers::RoomSubserverObject(networker, System::PortsManager::allocate_port());

		//Передаем субсерверу все данные о пользователе и переводим пользователя на этот субсервер
        Database::User userInfo = usersDb->get_user(user);
        current->pass_client(creator, UserStatistics(userInfo));
        std::string data = System::Serializer::serialize<Network::Client>(current->get_my_address());

		//Отправляем пользователю сообщение о переводе на субсервер
		networker->send_message(Network::Message(
									Network::MessageType_ChangeServer,
									(Network::SymbolType*)data.c_str(),
									data.length(),
									creator)
								);

		//Отправляем сообщение пользователю с ключем для входа в комнату
        std::string keyData = System::Serializer::serialize<QString>(current->get_key());

		networker->send_message(Network::Message(
									Network::MessageType_GameCreated,
									(Network::SymbolType*)keyData.c_str(),
									keyData.length(),
									creator)
								);
        games.append(current);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, creator);
		exception->close();
    }
}

void MainServerManager::get_statistics(Database::UserIdType user,
									   Network::Client asker,
									   Network::MessageIdType requestId)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType askerUser = get_user_by_client(asker);
		check_null_user(askerUser);

		//Тут я не уверен. Возможно, если пользователь хочет узнать свою статистику,
		//то ему надо дать несколько больше данных
		//if(askerUser == user){
		//	...
		//}

		//Берем все необходимые данные из БД
        Database::User currentUser = usersDb->get_user(user);

        UserStatistics statistics = UserStatistics(currentUser);

		//И отправляем их клиенту
        std::string data = System::Serializer::serialize<UserStatistics>(statistics);

		networker->send_message(Network::Message(
									Network::MessageType_RequestAnswer,
									(Network::SymbolType*)data.c_str(),
									data.length(),
									asker,
									requestId)
								);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, asker);
		exception->close();
    }

}

void MainServerManager::add_game(Gameplay::Game game, Subservers::RoomSubserverObject* rso)
{
    try {
		//Проверяем, что такая игра вообще была и удаляем ее
        if(games.contains(rso)){
            rso->finish_work();
            games.removeOne(rso);
			SAFE_DELETE( rso );
        } else{
			throw Exceptions::Exception::generate(
						System::String("Signal from unknown room subserver object"),
						Exceptions::MainServerExceptionId_NoSuchGame);
        }
		//Добавляем в БД необходимые данные
        Database::GameIdType current = gamesDb->add_game(game);
        game.id = current;
        usersDb->register_game(game);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		exception->show();
    }
}

void MainServerManager::confirm_email(Network::Client client,
									  QString confirmationKey,
									  Network::MessageIdType requestId)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);

		//Пробуем подставить этот ключ подтверждения
		bool success = usersDb->confirm_user(user, confirmationKey);

		//Отправляем пользователю данные о том, удалось или нет подтвердить почту
		System::String mesData = System::Serializer::serialize<bool>(success);

		networker->send_message(Network::Message(Network::MessageType_RequestAnswer,
												 (Network::SymbolType*)mesData.data,
												 mesData.size,
												 client,
												 requestId)
								);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }

}

void MainServerManager::logout_user(Network::Client client)
{
    try {
		//Проверяем, что клиент авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);
        if(users.contains(user)){
            users[users.indexOf(user)] = nullUser;
        }

		//Логаутимся
        usersDb->logout_user(user);
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		exception->close();
    }
}

void MainServerManager::get_logs_data(QString data, Network::Client sender)
{
	System::LogSource logSource = System::LogSource_Client;
	//Если это логи от субсерверов, то помечаем их другим источником
	if((unsigned)sender.ip != QHostAddress("127.0.0.1").toIPv4Address()){
		logSource = System::LogSource_Subserver;
	}

	//Собираем строку с информацией об источнике
	std::string senderInfo = QHostAddress(sender.ip).toString().toStdString()
			+ ":"
			+ QString::number(sender.port).toStdString();
	try {
		//Добавляем в файл логов
		System::LogsManager::add_record(data.toStdString(), logSource, senderInfo);
	} catch (Exceptions::Exception* exception) {
		exception->show();
	}

}

void MainServerManager::add_transaction(Database::Transaction transaction)
{
    try {
		//Добавляем транзакцию в БД
        Database::TransactionIdType id = transactionsDb->add_transaction(transaction.buyer, transaction.price, transaction.type);
        usersDb->add_transaction(transaction.buyer, id);

		//Меняем тип аккаунта покупателя
		switch (transaction.type) {
		case Database::TransactionType_BuySomething:{
			usersDb->set_account_type(transaction.buyer, Database::AccountType_Pro);
			break;
		}
		default:{
			throw Exceptions::Exception::generate(
							System::String("Unknown transaction type!"),
							Exceptions::MainServerExceptionId_UnknownTransactionType);
		}
		}
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		exception->close();
    }
}

void MainServerManager::change_nickname(Network::Client client, QString newNickname)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);
		//Меняем никнейм
        usersDb->change_nickname(user, newNickname);
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::change_email(Network::Client client, QString newEmail, Network::MessageIdType requestId)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);
		//Пробуем изменить email
        bool success = usersDb->change_email(user, newEmail);

		//Отправляем сообщение клиенту о том, удалось ли изменить email
		networker->send_message(Network::Message(
									Network::MessageType_RequestAnswer,
									(Network::SymbolType*)System::Serializer::serialize<bool>(success).c_str(),
									(int)sizeof(bool),
									client,
									requestId)
								);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::change_achievement(Database::UserIdType user, Database::Achievement achievement)
{
    try {
		//Меняем достижение пользователя
        usersDb->add_achievement(user, achievement);

		//Если пользователь онлайн, то отправим ему уведомление.
		//Если нет, то будет выброшено исключение и ничего не произойдет
        Network::Client client = get_client_by_user(user);
        std::string messageData = System::Serializer::serialize<Database::Achievement>(achievement);

		networker->send_message(Network::Message(
									Network::MessageType_AchievementChange,
									(Network::SymbolType*)messageData.c_str(),
									messageData.length(),
									client)
								);

    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		exception->close();
    }
}

void MainServerManager::add_user_to_game(Network::Client client, QString gameKey)
{
	try {
		//Проверяем все игры, к какой подойдет этот ключ
		for(int i = 0; i < games.length(); i++){
			if(games[i]->check_connection_key(gameKey)){
				//Проверяем, авторизован ли пользователь
				Database::UserIdType user = get_user_by_client(client);
				check_null_user(user);

				//Получаем информацию о пользователеи передаем ее на субсервер
				Database::User userInfo = usersDb->get_user(user);
				games[i]->pass_client(client, UserStatistics(userInfo));

				//Отправляем клиенту инфу о переходе на субсервер
				std::string data = System::Serializer::serialize<Network::Client>(games[i]->get_my_address());

				networker->send_message(Network::Message(
											Network::MessageType_ChangeServer,
											(Network::SymbolType*)data.c_str(),
											data.length(),
											client)
										);

				return;
			}
		}

		//Если ничего не нашли, то отправляем сообщение о неверном ключе
		networker->send_message(Network::Message(
									Network::MessageType_InvalidGameKey,
									(Network::SymbolType*)"",
									1,
									client)
								);

	} catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
	}

}

void MainServerManager::delete_message(Network::Client client, Database::MessageIdType message)
{
    try {
		//!Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);

        Database::ChatMessage messageData = chatsDb->get_message(message);
		//!Проверяем, может ли этот пользователь удалить это сообщение
        bool canEdit = false;
		//!Если это его собственное сообщение,
		//то он должен иметь право на редактирование сообщений в этом чате
        if(user == messageData.from){
            canEdit = chatSettingsDb->can_send_message(user, messageData.toChat);
		//Если это не его сообщение, то он должен иметь право
		//на редактирование чужих сообщений в этом чате
        } else{
            canEdit = chatSettingsDb->can_edit_messages(user, messageData.toChat);
        }

		//Если он может, то удаляем. Если нет, то отправляем ему сообщение о запрете
        if(canEdit){
            chatsDb->delete_message(message);
        } else{
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't edit this message",
										29,
										client)
									);
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::edit_message(Network::Client client, Database::ChatMessage message)
{
    try {
		//Проверяем, что пользователь авторизован
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);

		//Проверяем, может ли пользователь редактировать это сообщение
        bool canEdit = false;
        if(user == message.from){
			//Если это его сообщение, то он должен иметь право на отправку сообщений в этот чат
            canEdit = chatSettingsDb->can_send_message(user, message.toChat);
        } else{
			//Если это чужое сообщение, то он должен иметь разрешение на редактирование чужих сообщений
            canEdit = chatSettingsDb->can_edit_messages(user, message.toChat);
        }

		//Если может, то редактируем. Если нет, то отправляем ему сообщение о запрете
        if(canEdit){
            chatsDb->edit_message(message);
        } else{
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't edit this message",
										29,
										client)
									);
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::read_message(Network::Client client, Database::MessageIdType message)
{
	try {
		//Проверяем, авторизован ли пользователь
        Database::UserIdType user = get_user_by_client(client);
		check_null_user(user);

		//Проверяем, может ли этот пользователь читать сообщение из этого чата
        Database::ChatMessage messageData = chatsDb->get_message(message);
        bool canEdit = chatSettingsDb->can_read_message(user, messageData.toChat);

        if(canEdit){
            chatsDb->message_read(message, user);
        } else{
			networker->send_message(Network::Message(
										Network::MessageType_AccessDenied,
										(Network::SymbolType*)"You can't edit this message",
										29,
										client)
									);
        }
    } catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception, client);
		exception->close();
    }
}

void MainServerManager::connect_to_processor()
{
	//Просто соединяем все слоты с сигналами...
    Network::MessageProcessor* processor = new Network::MessageProcessor(networker);

    connect(processor, &Network::MessageProcessor::login_user, this, &MainServerManager::login_user);

    connect(processor, &Network::MessageProcessor::send_chat_message, this, &MainServerManager::send_chat_message);

    connect(processor, &Network::MessageProcessor::create_user, this, &MainServerManager::create_user);

    connect(processor, &Network::MessageProcessor::create_chat, this, &MainServerManager::create_chat);

    connect(processor, &Network::MessageProcessor::get_last_messages, this, &MainServerManager::get_last_messages);

    connect(processor, &Network::MessageProcessor::get_users_chats, this, &MainServerManager::get_users_chats);

    connect(processor, &Network::MessageProcessor::add_user_to_chat, this, &MainServerManager::add_user_to_chat);

    connect(processor, &Network::MessageProcessor::remove_user_from_chat, this, &MainServerManager::remove_user_from_chat);

    connect(processor, &Network::MessageProcessor::change_users_chat_capability,
            this, &MainServerManager::change_users_chat_capability);

    connect(processor, &Network::MessageProcessor::create_game, this, &MainServerManager::create_game);

    connect(processor, &Network::MessageProcessor::get_statistics, this, &MainServerManager::get_statistics);

    connect(processor, &Network::MessageProcessor::confirm_email, this, &MainServerManager::confirm_email);

    connect(processor, &Network::MessageProcessor::logout_user, this, &MainServerManager::logout_user);

    connect(processor, &Network::MessageProcessor::get_logs_data, this, &MainServerManager::get_logs_data);

    connect(processor, &Network::MessageProcessor::change_nickname, this, &MainServerManager::change_nickname);

    connect(processor, &Network::MessageProcessor::change_email, this, &MainServerManager::change_email);

    connect(processor, &Network::MessageProcessor::add_user_to_game, this, &MainServerManager::add_user_to_game);

    connect(processor, &Network::MessageProcessor::delete_message, this, &MainServerManager::delete_message);

    connect(processor, &Network::MessageProcessor::edit_message, this, &MainServerManager::edit_message);

    connect(processor, &Network::MessageProcessor::read_message, this, &MainServerManager::read_message);
}

void MainServerManager::_get_data_from_request(Requests::NetworkRequest *req)
{
    if(req != 0){
        try {
            req->sleep_untill_finished();
            std::cout << "The answer is " << req->get_result<int>() << std::endl;
        } catch (Exceptions::Exception* exception) {
            exception->show();
        }
    }
}

void MainServerManager::_database_get_all()
{
	try {
		std::cout << "DATABASE DATA\n";

		MafiaList<Database::User> allUsers = usersDb->get_users();

		for(int i = 0; i < allUsers.length(); i++){
			allUsers[i].show();
		}

		MafiaList<Database::Transaction> allTransactions = transactionsDb->get_all_transactions();

		for(int i = 0; i < allTransactions.length(); i++){
			allTransactions[i].show();
		}

		MafiaList<Database::Chat> allChats = chatSettingsDb->get_all_chats();

		for(int i = 0; i < allChats.length(); i++){
			allChats[i].show();
		}

		MafiaList<Gameplay::Game> allGames = gamesDb->get_games_with();

		for(int i = 0; i < allGames.length(); i++){
			allGames[i].show();
		}

		MafiaList<Database::ChatMessage> allMessages = chatsDb->get_messages();

		for(int i = 0; i < allMessages.length(); i++){
			allMessages[i].show();
		}

		std::cout << "END DATABASE DATA\n";
	} catch (Exceptions::Exception* exception) {
		standart_exception_processing(exception);
		exception->show();
	}

}

Network::Client MainServerManager::get_client_by_user(Database::UserIdType user)
{
    if(users.contains(user)){
        return clients[users.indexOf(user)];
    } else{
		throw Exceptions::Exception::generate(
					System::String("No such user in users list!!!"),
					Exceptions::MainServerExceptionId_NoSuchUser);
        return Network::Client();
    }
}

Database::UserIdType MainServerManager::get_user_by_client(Network::Client client)
{
    if(clients.contains(client)){
        return users[clients.indexOf(client)];
    } else{
		throw Exceptions::Exception::generate(
					System::String("No such client in clients list!!!"),
					Exceptions::MainServerExceptionId_NoSuchClient);
        return Database::UserIdType();
	}
}

void MainServerManager::send_error_email(QString message)
{
	for(int receiverIndex = 0; receiverIndex < errorEmailReceivers.length(); receiverIndex++){
		System::MailSender::send_email(
				errorEmailReceivers[ receiverIndex ],
				"ERROR IN SERVER!!!",
				message);
	}
}

void MainServerManager::solve_database_error()
{
	bool restartSuccess = false;
	while(!restartSuccess){
		try {
			//Пробуем перезапустить БД. Если все плохо, то будет выброшено исключение
			restartSuccess = dbWorker->restart_database();
		} catch (Exceptions::Exception* dbException) {
			switch (dbException->get_id()) {
				case Exceptions::DatabaseWorkingExceptionId_DatabaseRestartAttemptsLimit:{
					//Отправляем нужным людям email о том, что все плохо
					send_error_email(
						QString::fromStdString(
							std::string(
								dbException->get_data().data,
								dbException->get_data().size)) +
						"\nException id : " +
						QString::number(dbException->get_id())
						);
					restartSuccess = true;
					dbException->close();
					break;
				}
				default:{
					dbException->show();
				}
			}
		}
	}
}

void MainServerManager::solve_users_problem(Database::UserIdType user)
{
	if(users.contains(user)){
		solve_users_problem( clients[ users.indexOf( user ) ] );
	}
}

void MainServerManager::solve_users_problem(Network::Client client)
{
	//Просто отправляем сообщение пользователю
	QString messageData = "Some error with your request!";
	std::string data = System::Serializer::serialize<QString>(messageData);
	networker->send_message(Network::Message(
								Network::MessageType_Error,
								(Network::SymbolType* )data.c_str(),
								data.length(),
								client
								)
							);
}

void MainServerManager::standart_exception_processing(Exceptions::Exception* exception)
{
	switch (exception->get_id()) {
		case Exceptions::DatabaseWorkingExceptionId_SQlQuery:{
			exception->show();
			break;
		}
		case Exceptions::DatabaseWorkingExceptionId_TimeLimit:{
			solve_database_error();
			break;
		}
	}
}

void MainServerManager::standart_exception_processing(Exceptions::Exception* exception, Database::UserIdType user)
{
	standart_exception_processing(exception);
	if(exception->get_id() == Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult){
		solve_users_problem(user);
	}
}

void MainServerManager::standart_exception_processing(Exceptions::Exception* exception, Network::Client client)
{
	standart_exception_processing(exception);
	switch(exception->get_id()){
		case Exceptions::MainServerExceptionId_NullUserRequest:{
			send_unauthorized_exception(client);
			break;
		}
		case Exceptions::MainServerExceptionId_NoSuchClient:{
			clients.append(client);
			users.append(nullUser);
			send_unauthorized_exception(client);
			break;
		}
		case Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult:{
			solve_users_problem(client);
			break;
		}
	}
}

void MainServerManager::check_null_user(Database::UserIdType user)
{
	if(user == nullUser){
		throw Exceptions::Exception::generate(
					System::String("Client is not authorized"),
					Exceptions::MainServerExceptionId_NullUserRequest);
	}
}

void MainServerManager::send_unauthorized_exception(Network::Client client)
{
	try {
		//Просто отправляем пользователю сообщение
		QString message = "You have to be authorized for this action";
		std::string data = System::Serializer::serialize<QString>(message);
		networker->send_message(Network::Message(
									Network::MessageType_Error,
									(Network::SymbolType*)data.c_str(),
									data.length(),
									client)
								);
	} catch (Exceptions::Exception* exception) {
		exception->show();
	}
}
