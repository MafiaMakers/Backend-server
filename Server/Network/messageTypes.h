#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H
#include "message.h"
#include "System/functions.h"
namespace Mafia {
    namespace Network {
        enum MessageType{
            //! \brief Нижняя граница типов сообщений
            MessageType_First = 1,
            //! \brief Просто текстовое сообщение
            MessageType_Text = 1,
            //! \brief Текстовое сообщение, не требующее подтверждения
            MessageType_NoConfirmText = 2,
            //! \brief Подтверждающее сообщение
            MessageType_Confirmation = 3,
            //! \brief Сообщение с ответом на запрос
            MessageType_RequestAnswer = 4,
            //! \brief Сообщение с абстрактным (каким-то) запросом. Пока сделано просто для тестов
            MessageType_AbstractRequest = 5,
            //! \brief Сообщение для передачи субсерверу клиента
            MessageType_PassClientRequest = 6,
            //! \brief Сообщение для проверки соединения (с субсерверами и с клиентами)
            MessageType_CheckConnection = 7,
            //! \brief Сообщение для проверки соединения с основным сервером (отправляется субсервером-восстановителем)
            MessageType_CheckConnectionServer = 8,
            //! \brief Сообщение для отправки сообщения в чат
            MessageType_SendChatMessage = 9,
            //! \brief Сообщение для изменения сообщения в чате
            MessageType_EditChatMessage = 10,
            //! \brief Сообщение для удаления сообщения из чата
            MessageType_DeleteChatMessage = 11,
            //! \brief Сообщение для уведомление о том, что игрок прочитал сообщение
            MessageType_ReadChatMessage = 12,
            //! \brief Сообщение, говорящее о том, что не удалось создать пользователя с такими данными (скорее всего это связано с тем, что email уже занят)
            MessageType_UnableToCreateUser = 13,
            //! \brief Сообщение, говорящее о том, что пользователь успешно вошел (сообщение имеет такую структуру:
            //! 4 байта - int, id пользователя, далее все остальные байты - email, под которым вошел пользователь)
            MessageType_LoggedIn = 14,
            //! \brief Сообщение, сообщающее о том, что пользователь успешно вышел из аккаунта (в сообщении не передается ничего)
            MessageType_LoggedOut = 15,
            //! \brief Сообщение, говорящее о том, что у пользователя недостаточно прав для совершения запрашиваемой операции
            //! (в сообщении строкой передается причина, по которой пользователь не получил разрешения)
            MessageType_AccessDenied = 16,
            //! \brief Сообщение, которое отправляется субсервером, чтобы получить последние сообщения из какого-либо чата
            MessageType_GetChatMessages = 17,
            //! \brief Сообщение, в котором передаются последние сообщения чата (начиная с самого последнего и сортированы по убыванию даты)
            //! Структура сообщения - это список \ref Mafia::Database::ChatMessage , сериализованный системным сериализатором
            MessageType_LastChatMessages = 18,
            //! \brief Сообщение, в котором передаются чаты пользователя (могут передаваться не все чаты)
            //! они отсортированы в порядке убывания даты последнего сообщения в этом чате и гарантируется, что передаются чаты,
            //! в которых сообщения были самыми последними
            MessageType_Chats = 19,
            //! \brief Сообщение, которое отправляется пользователю при получении нового достижения (нашивки)
            MessageType_AchievementChange = 20,
            //! \brief Сообщение, которое отправляет субсервер-комната основному серверу, если пользователь покидает комнату
            MessageType_PlayerLeaveGame = 21,
            //! \brief Сообщение, в котором субсервер передает основному серверу результаты завершившейся игры
            MessageType_GameResults = 22,
            //! \brief Сообщение, которое сервер отправляет клиенту, когда тот должен перейти на новый сервер
            MessageType_ChangeServer = 23,
            //! \brief Сообщение, которое отправляется клиенту, который создал игру
            //! (в сообщении хранится ключ игры, который надо ввести для подключения)
            MessageType_GameCreated = 24,
            //! \brief Сообщение, которое отправляется игроку, который ввел неверный ключ для входа в игру
            MessageType_InvalidGameKey = 25,

            MessageType_CreateUser = 26,

            MessageType_LoginRequest = 27,

            MessageType_CreateChatRequest = 28,

            MessageType_GetChatsRequest = 29,

            MessageType_AddUserToChat = 30,

            MessageType_ChangeUsersCapability = 31,

            MessageType_RemoveUserFromChat = 32,

            MessageType_CreateGameRequest = 33,

            MessageType_GetStatisticsRequest = 34,

            MessageType_ConfirmEmail = 35,

            MessageType_Logout = 36,

            MessageType_LogsData = 37,

            MessageType_ChangeNickname = 38,

            MessageType_ChangeEmailRequest = 39,

            MessageType_GetIntoGameRequest = 40,

            MessageType_NewChatMessage = 41,

            MessageType_EditedChatMessage = 42,

            MessageType_DeletedChatMessage = 43,

            MessageType_OnReadChatMessage = 44,
            //! \brief Верхняя граница типов сообщений
            MessageType_Last = 45
        };


    }
}

#endif // MESSAGETYPES_H
