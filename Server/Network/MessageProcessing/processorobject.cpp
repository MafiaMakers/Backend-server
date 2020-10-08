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

ProcessorObject *ProcessorObject::generate(Message_t message)
{
	ProcessorObject* result;
	switch (message.id) {
    case MessageType_CreateUser:{
		SAFE_NEW(result, CreateUserProcessorObject(message));
		break;
    }
    case MessageType_SendChatMessage:{
		SAFE_NEW(result, SendChatMessageProcessorObject(message));
		break;
    }
    case MessageType_EditChatMessage:{
		SAFE_NEW(result, EditChatMessageProcessorObject(message));
		break;
    }
    case MessageType_DeleteChatMessage:{
		SAFE_NEW(result, DeleteChatMessageProcessorObject(message));
		break;
    }
    case MessageType_ReadChatMessage:{
		SAFE_NEW(result, ReadChatMessageProcessorObject(message));
		break;
    }
    case MessageType_GetChatMessages:{
		SAFE_NEW(result, GetChatMessagesProcessorObject(message));
		break;
    }
    case MessageType_LoginRequest:{
		SAFE_NEW(result, LoginRequestProcessorObject(message));
		break;
    }
    case MessageType_CreateChatRequest:{
		SAFE_NEW(result, CreateChatRequestProcessorObject(message));
		break;
    }
    case MessageType_GetChatsRequest:{
		SAFE_NEW(result, GetChatsRequestProcessorObject(message));
		break;
    }
    case MessageType_AddUserToChat:{
		SAFE_NEW(result, AddUserToChatProcessorObject(message));
		break;
    }
    case MessageType_RemoveUserFromChat:{
		SAFE_NEW(result, RemoveUserFromChatProcessorObject(message));
		break;
    }
    case MessageType_ChangeUsersCapability:{
		SAFE_NEW(result, ChangeUsersCapabilityProcessorObject(message));
		break;
    }
    case MessageType_CreateGameRequest:{
		SAFE_NEW(result, CreateGameRequestProcessorObject(message));
		break;
    }
    case MessageType_GetStatisticsRequest:{
		SAFE_NEW(result, GetStatisticsRequestProcessorObject(message));
		break;
    }
    case MessageType_ConfirmEmail:{
		SAFE_NEW(result, ConfirmEmailProcessorObject(message));
		break;
    }
    case MessageType_Logout:{
		SAFE_NEW(result, LogoutProcessorObject(message));
		break;
    }
    case MessageType_LogsData:{
		SAFE_NEW(result, LogsDataProcessorObject(message));
		break;
    }
    case MessageType_ChangeNickname:{
		SAFE_NEW(result, ChangeNicknameProcessorObject(message));
		break;
    }
    case MessageType_ChangeEmailRequest:{
		SAFE_NEW(result, ChangeEmailRequestProcessorObject(message));
		break;
    }
    case MessageType_GetIntoGameRequest:{
		SAFE_NEW(result, GetIntoGameRequestProcessorObject(message));
		break;
    }
    default:{
		throw Exceptions::Exception::generate(System::String("Unknown message type!"),
                                                         Exceptions::MessageProcessingExceptionId_UnknownMessageType);
        return NULL;
    }
    }

	return result;
}

ProcessorObject::~ProcessorObject()
{

}

template<>
int ProcessorObject::get_from_json(QJsonObject obj, QString key){

	return get_json_value(obj, key).toInt();
}

template<>
QString ProcessorObject::get_from_json(QJsonObject obj, QString key){
	return get_json_value(obj, key).toString();
}

template<>
std::string ProcessorObject::get_from_json(QJsonObject obj, QString key){
	return get_json_value(obj, key).toString().toStdString();
}

template<>
System::String ProcessorObject::get_from_json(QJsonObject obj, QString key){
	return System::String(get_json_value(obj, key).toString().toStdString());
}

template<>
MafiaList<int> ProcessorObject::get_from_json(QJsonObject obj, QString key){
	QJsonArray arr = get_json_value(obj, key).toArray();
	auto result = MafiaList<int>();
	for(int i = 0; i < arr.size(); i++){
		result.append(arr.at(i).toInt());
	}
	return result;
}

ProcessorObject::ProcessorObject(Message_t message)
{
    this->id = message.id;
	this->data = message.data;
	this->sender = message.sender;
}

QJsonValue ProcessorObject::get_json_value(QJsonObject obj, QString key)
{
	QJsonValue val = obj.value(key);

	if(val == QJsonValue::Undefined){
		throw Exceptions::Exception::generate(System::String("No " + key.toStdString() + " in json!!!"),
											  Exceptions::MessageParsingExceptionId_NoSuchID);
	}
	return val;
}
