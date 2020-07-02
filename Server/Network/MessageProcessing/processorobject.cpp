#include "processorobject.h"
#include "Exceptions/messageprocessingexception.h"

#include "createuserprocessorobject.h"
#include "sendchatmessageprocessorobject.h"
#include "editchatmessageprocessorobject.h"
#include "deletechatmessageprocessorobject.h"
#include "readchatmessageprocessorobject.h"
#include "getchatmessagesprocessorobject.h"
#include "loginrequestprocessorobject.h"
#include "createchatrequestprocessorobject.h"
#include "getchatsrequestprocessorobject.h"
#include "addusertochatprocessorobject.h"
#include "removeuserfromchatprocessorobject.h"
#include "changeuserscapabilityprocessorobject.h"
#include "creategamerequestprocessorobject.h"
#include "getstatisticsrequestprocessorobject.h"
#include "confirmemailprocessorobject.h"
#include "logoutprocessorobject.h"
#include "logsdataprocessorobject.h"
#include "changenicknameprocessorobject.h"
#include "changeemailrequestprocessorobject.h"
#include "getintogamerequestprocessorobject.h"

using namespace Mafia;
using namespace Network;
using namespace MessageProcessing;

ProcessorObject *ProcessorObject::generate(Message message)
{
    switch (message.type) {
    case MessageType_CreateUser:{
        return new CreateUserProcessorObject(message);
    }
    case MessageType_SendChatMessage:{
        return new SendChatMessageProcessorObject(message);
    }
    case MessageType_EditChatMessage:{
        return new EditChatMessageProcessorObject(message);
    }
    case MessageType_DeleteChatMessage:{
        return new DeleteChatMessageProcessorObject(message);
    }
    case MessageType_ReadChatMessage:{
        return new ReadChatMessageProcessorObject(message);
    }
    case MessageType_GetChatMessages:{
        return new GetChatMessagesProcessorObject(message);
    }
    case MessageType_LoginRequest:{
        return new LoginRequestProcessorObject(message);
    }
    case MessageType_CreateChatRequest:{
        return new CreateChatRequestProcessorObject(message);
    }
    case MessageType_GetChatsRequest:{
        return new GetChatsRequestProcessorObject(message);
    }
    case MessageType_AddUserToChat:{
        return new AddUserToChatProcessorObject(message);
    }
    case MessageType_RemoveUserFromChat:{
        return new RemoveUserFromChatProcessorObject(message);
    }
    case MessageType_ChangeUsersCapability:{
        return new ChangeUsersCapabilityProcessorObject(message);
    }
    case MessageType_CreateGameRequest:{
        return new CreateGameRequestProcessorObject(message);
    }
    case MessageType_GetStatisticsRequest:{
        return new GetStatisticsRequestProcessorObject(message);
    }
    case MessageType_ConfirmEmail:{
        return new ConfirmEmailProcessorObject(message);
    }
    case MessageType_Logout:{
        return new LogoutProcessorObject(message);
    }
    case MessageType_LogsData:{
        return new LogsDataProcessorObject(message);
    }
    case MessageType_ChangeNickname:{
        return new ChangeNicknameProcessorObject(message);
    }
    case MessageType_ChangeEmailRequest:{
        return new ChangeEmailRequestProcessorObject(message);
    }
    case MessageType_GetIntoGameRequest:{
        return new GetIntoGameRequestProcessorObject(message);
    }
    default:{
        throw new Exceptions::MessageProcessingException(System::String("Unknown message type!"),
                                                         Exceptions::MessageProcessingExceptionId_UnknownMessageType);
        return NULL;
    }
    }
}

ProcessorObject::ProcessorObject(Message message)
{
    this->id = message.id;
    this->data = System::String((char*)message.data, sizeof(SymbolType) * message.size);
    this->sender = message.client;
}
