#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H
#include "message.h"
#include "System/functions.h"
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
    //! \brief Верхняя граница типов сообщений
    MessageType_Last = 13
};


#endif // MESSAGETYPES_H
