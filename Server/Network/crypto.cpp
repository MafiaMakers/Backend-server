#include "crypto.h"
#include "Exceptions/messageparsingexception.h"
#include "System/serializer.h"
#include "System/tuple.h"

using namespace Mafia;
using namespace Network;
System::LimitedQueue<MessageIdType>* Crypto::lastMessageIds = new System::LimitedQueue<MessageIdType>(rememberMessagesCount);
System::String Crypto::key = System::String();

void Crypto::setKey(System::String key){
    lastMessageIds = new System::LimitedQueue<MessageIdType>(rememberMessagesCount);
    Crypto::key = key;
}

void Crypto::setKey(std::string key){
    Crypto::key = System::String(key);
}

void Crypto::setKey(char* key, int size){
    Crypto::key = System::String(key, size);
}

Message Crypto::parse_data(char *data, int size){
    System::String tData = System::String(0, 0);
    try {
        tData = Crypto::_decrypt(System::String(data, size));
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::MessageParsingExceptionId_ShortMessage:{
            exception->show();
            break;
        }
        default:{
            throw exception;
        }
        }
    }

    System::Tuple<Message, ControlSumType> result = System::Serializer::deserialize<System::Tuple<Message, ControlSumType>>(
                tData);

    ControlSumType realSum = 0;
    for(unsigned int i = 0; i < result.item1.size * sizeof(SymbolType); i++){
        realSum += (int)(char)((char*)result.item1.data)[i];
    }

    if(result.item2 != realSum){
		throw new Exceptions::MessageParsingException(System::String("control sum doesn't match to real sum " +
																	 QString::number(result.item2).toStdString() + " " +
																	 QString::number(realSum).toStdString()),
													  Exceptions::MessageParsingExceptionId_ControlSumMismatch);
    }

    Crypto::lastMessageIds->append(result.item1.id);

    return result.item1;
}

Message Crypto::parse_data(System::String data){
    try {
        return parse_data(data.data, data.size);
    } catch (Exceptions::Exception* exception) {
        throw exception;
    }
}

System::String Crypto::wrap_message(Message mes){

    ControlSumType controlSum = 0;
    for(int i = 0; i < mes.size; i++){
        for(unsigned int j = 0; j < sizeof (SymbolType); j++){
            controlSum += (ControlSumType)(((char*)&mes.data[i])[j]);
        }
    }
    System::String data = System::String(System::Serializer::serialize<System::Tuple<Message, ControlSumType>>(
                System::Tuple<Message, ControlSumType>(mes, controlSum)));

    System::String result;
    try {
        result = Crypto::_encrypt(data);
    } catch (Exceptions::Exception* exception) {
        throw exception;
    }

    return result;
}

bool Crypto::_message_id_ok(MessageIdType id)
{
    return true;
}

System::String Crypto::_encrypt(System::String decrypted){
    if(Crypto::key.size == 0){
        throw new Exceptions::MessageParsingException(System::String("encryption failed! Key is not set"), Exceptions::MessageParsingExceptionId_NoneKey);
    }
    System::String encrypted = System::String(new char[decrypted.size], decrypted.size);
    for(int i = 0; i < decrypted.size; i++){
        short num = (short)decrypted.data[i] + (short)Crypto::key.data[i % Crypto::key.size];
        if(num > 128){
            num -= 256;
        }
        if(num < -128){
            num += 256;
        }
        encrypted.data[i] = (char)num;
    }

    return encrypted;
}

System::String Crypto::_decrypt(System::String encrypted){
    if(Crypto::key.size == 0){
        throw new Exceptions::MessageParsingException(System::String("decryption failed! Key is not set"), Exceptions::MessageParsingExceptionId_NoneKey);
    }
    System::String decrypted = System::String(new char[encrypted.size], encrypted.size);
    for(int i = 0; i < decrypted.size; i++){
        short num = (short)encrypted.data[i] - (short)Crypto::key.data[i % Crypto::key.size];
        if(num > 128){
            num -= 256;
        }
        if(num < -128){
            num += 256;
        }
        decrypted.data[i] = (char)(num);
    }

    return decrypted;
}
