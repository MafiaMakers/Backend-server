#include "crypto.h"
#include "Exceptions/messageparsingexception.h"

using namespace Mafia;

LimitedQueue<MessageIdType>* Crypto::lastMessageIds = new LimitedQueue<MessageIdType>(rememberMessagesCount);
String Crypto::key = String();

void Crypto::setKey(String key){
    lastMessageIds = new LimitedQueue<MessageIdType>(rememberMessagesCount);
    Crypto::key = key;
}

void Crypto::setKey(std::string key){
    Crypto::key = String(key);
}

void Crypto::setKey(char* key, int size){
    Crypto::key = String(key, size);
}

Message Crypto::parse_data(char *data, int size){
    String tData = String(0, 0);
    try {
        tData = Crypto::_decrypt(String(data, size));
    } catch (Exception* exception) {
        switch (exception->get_id()) {
        case SHORT_MESSAGE_EXCEPTION_ID:{
            exception->show();
            break;
        }
        default:{
            throw exception;
        }
        }
    }

    int othersCount = tData.size - (sizeof(MessageIdType) + sizeof (MessageTypeType) + sizeof (ControlSumType));

    if(othersCount < 0){
        throw new MessageParsingException(String("decrypted data size is too small"), SHORT_MESSAGE_EXCEPTION_ID);
    }
    Message result = Message();
    int currentInd = 0;
    char* mesIdChar = new char[sizeof(MessageIdType)];
    for(unsigned int i = 0; i < sizeof(MessageIdType); i++){
        mesIdChar[i] = tData.data[currentInd];
        currentInd++;
    }
    result.id = *(MessageIdType*)mesIdChar;
    if(!Crypto::_message_id_ok(result.id)){
        throw new MessageParsingException(String("message id is less or equal to previous"), INVALID_MESSAGE_ID_EXCEPTION_ID);
    }

    char* mesTypeChar = new char[sizeof (MessageTypeType)];
    for (unsigned int i = 0; i < sizeof(MessageTypeType); i++){
        mesTypeChar[i] = tData.data[currentInd];
        currentInd++;
    }

    result.type = *(MessageTypeType*)mesTypeChar;
    char* controlSumChar = new char[sizeof (ControlSumType)];
    for(unsigned int i = 0; i < sizeof(ControlSumType); i++){
        controlSumChar[i] = tData.data[currentInd];
        currentInd++;
    }

    ControlSumType controlSum = *(ControlSumType*)controlSumChar;
    if(othersCount % sizeof(SymbolType) != 0){
        throw new MessageParsingException(String("message size doesn't multiple to size of SymbolType"), INVALID_MESSAGE_SIZE_EXCEPTION_ID);
    }

    result.data = new SymbolType[othersCount / sizeof (SymbolType)];

    ControlSumType realSum = 0;

    result.size = othersCount / sizeof (SymbolType);

    for(unsigned int i = 0; i < othersCount / sizeof (SymbolType); i++){
        char* symbolChar = new char[sizeof(SymbolType)];
        for(unsigned int j = 0; j < sizeof (SymbolType); j++){
            symbolChar[j] = tData.data[currentInd];
            realSum += (ControlSumType)tData.data[currentInd];
            currentInd++;
        }
        result.data[i] = *(SymbolType*)symbolChar;
        delete[] symbolChar;
    }

    if(controlSum != realSum){
        throw new MessageParsingException(String("control sum doesn't match to real sum"), CONTROL_SUM_EXCEPTION_ID);
    }

    Crypto::lastMessageIds->append(result.id);

    return result;
}

Message Crypto::parse_data(String data){
    try {
        return parse_data(data.data, data.size);
    } catch (Exception* exception) {
        throw exception;
    }
}

String Crypto::wrap_message(Message mes){
    unsigned int size = (sizeof(MessageIdType) + sizeof (MessageTypeType) + sizeof (ControlSumType))+ mes.size * sizeof (SymbolType);

    String resultData = String(new char[size], size);

    unsigned int currentInd = 0;
    for(unsigned int i = 0; i < sizeof(MessageIdType); i++){
        resultData.data[currentInd] = ((char*)(&(mes.id)))[i];
        currentInd++;
    }

    for(unsigned int i = 0; i < sizeof(MessageTypeType); i++){
        resultData.data[currentInd] = ((char*)(&(mes.type)))[i];
        currentInd++;
    }

    currentInd += sizeof (ControlSumType);

    ControlSumType controlSum = 0;
    for(int i = 0; i < mes.size; i++){
        for(unsigned int j = 0; j < sizeof (SymbolType); j++){
            resultData.data[currentInd] = ((char*)&mes.data[i])[j];
            controlSum += (ControlSumType)(((char*)&mes.data[i])[j]);
            currentInd++;
        }
    }

    for(unsigned int i = 0; i < sizeof (ControlSumType); i++){
        resultData.data[sizeof(MessageIdType) + sizeof(MessageTypeType) + i] = ((char*)&controlSum)[i];
    }

    String result;
    try {
        result = Crypto::_encrypt(resultData);
    } catch (Exception* exception) {
        throw exception;
    }

    return result;
}

bool Crypto::_message_id_ok(MessageIdType id)
{
    return(!(lastMessageIds->contains(id)) || id == -1);
}

String Crypto::_encrypt(String decrypted){
    if(Crypto::key.size == 0){
        throw new MessageParsingException(String("encryption failed! Key is not set"), NONE_KEY_EXCEPTION_ID);
    }
    String encrypted = String(new char[decrypted.size], decrypted.size);
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

String Crypto::_decrypt(String encrypted){
    if(Crypto::key.size == 0){
        throw new MessageParsingException(String("decryption failed! Key is not set"), NONE_KEY_EXCEPTION_ID);
    }
    String decrypted = String(new char[encrypted.size], encrypted.size);
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
