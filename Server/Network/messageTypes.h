#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H
#include "message.h"

enum MessageType{
    //! Нижняя граница типов сообщений
    MessageType_First = 1,
    //! Просто текстовое сообщение
    MessageType_Text = 1,
    //! Текстовое сообщение, не требующее подтверждения
    MessageType_NoConfirmText = 2,
    //! Подтверждающее сообщение
    MessageType_Confirmation = 3,
    //! Сообщение с ответом на запрос
    MessageType_RequestAnswer = 4,
    //! Сообщение с абстрактным (каким-то) запросом. Пока сделано просто для тестов
    MessageType_AbstractRequest = 5,
    //! Сообщение для передачи субсерверу клиента
    MessageType_PassClientRequest = 6,
    //! Сообщение для проверки соединения (с субсерверами и с клиентами)
    MessageType_CheckConnection = 7,
    //! Верхняя граница типов сообщений
    MessageType_Last = 8
};


#endif // MESSAGETYPES_H
