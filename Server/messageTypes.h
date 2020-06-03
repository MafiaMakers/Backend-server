#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H
#include "message.h"

//! Просто текстовое сообщение
#define TEXT_MESSAGE_TYPE ((MessageTypeType)1)
//! Текстовое сообщение, не требующее подтверждения
#define NO_CINFIRM_TEXT_MESSAGE_TYPE ((MessageTypeType)2)
//! Подтверждающее сообщение
#define CONFIRMATION_MESSAGE_TYPE ((MessageTypeType)3)
//! Сообщение с ответом на запрос
#define REQUEST_ANSWER_MESSAGE_TYPE ((MessageTypeType)4)
//! Сообщение с абстрактным (каким-то) запросом. Пока сделано просто для тестов
#define ABSTRACT_REQUEST_MESSAGE_TYPE ((MessageTypeType)5)

#endif // MESSAGETYPES_H
