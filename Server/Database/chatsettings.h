#ifndef CHATSETTINGS_H
#define CHATSETTINGS_H

#include "user.h"

namespace Mafia {
	namespace Database {
		//! \brief Перечисление возможных должностей пользователя в чате
		enum ChatCapability{
			//! \brief Индекс первого элемента перечисления (см. )
			ChatCapabilities_First = 0,
			//! \brief Просто наблюдатель (не может отправлять сообщения)
			ChatCapabilities_Watcher = 0,
			//! \brief Обычный пользователь (может читать и отправлять сообщения)
			ChatCapabilities_Speaker = 1,
			//! \brief Может редактировать свои и чужие сообщения
			ChatCapabilities_Editor = 2,
			//! \brief Админ (может добавлять, удалять, назначать должности разных пользователей)
			ChatCapabilities_Admin = 3,
			//! \brief Никакая роль (используется для обозначения роли пользователя, не состоящего в чате)
			ChatCapabilities_None = 4,
			//! \brief Индекс последнего элемента перечисления (см. )
			ChatCapabilities_Last = 4
		};
		//! \brief Структура для хранения данных чата
		struct Chat{
			//! \brief id чата
			ChatIdType id;
			//! \brief Список id всех пользователей, состоящих в чате
			MafiaList<UserIdType> users;
			//! \brief Список должностей пользователей (usersCapabilities[i] - это должность i-го пользователя)
			MafiaList<ChatCapability> usersCapabilities;
			//! \brief Дата и время создания чата
			DATE_TIME creationTime;
			//! \brief Функция для вывода в консоль всех полей чата
			void show();

			bool operator == (const Chat &a) const;

			bool operator != (const Chat &a) const;
		};

	}
}

#endif // CHATSETTINGS_H
