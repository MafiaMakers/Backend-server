#include "crypto.h"
#include "Exceptions/messageparsingexception.h"

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

    int othersCount = tData.size - (sizeof(MessageIdType) + sizeof (MessageTypeType) + sizeof (ControlSumType) + sizeof(Message::partIndex) + sizeof (Message::partsCount));

    if(othersCount < 0){
        throw new Exceptions::MessageParsingException(System::String("decrypted data size is too small"), Exceptions::MessageParsingExceptionId_ShortMessage);
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
        throw new Exceptions::MessageParsingException(System::String("message id is less or equal to previous"), Exceptions::MessageParsingExceptionId_InvalidMessageId);
    }

    char* mesTypeChar = new char[sizeof (MessageTypeType)];
    for (unsigned int i = 0; i < sizeof(MessageTypeType); i++){
        mesTypeChar[i] = tData.data[currentInd];
        currentInd++;
    }

    result.type = *(MessageTypeType*)mesTypeChar;

    char* mesPartIdxChar = new char[sizeof (Message::partIndex)];
    for(unsigned int i = 0; i < sizeof (Message::partIndex); i++){
        mesPartIdxChar[i] = tData.data[currentInd];
        currentInd++;
    }
    result.partIndex = *(int*)mesPartIdxChar;

    char* mesPartsCount = new char[sizeof (Message::partsCount)];
    for(unsigned int i = 0; i < sizeof (Message::partsCount); i++){
        mesPartsCount[i] = tData.data[currentInd];
        currentInd++;
    }
    result.partsCount = *(int*)mesPartsCount;

    char* controlSumChar = new char[sizeof (ControlSumType)];
    for(unsigned int i = 0; i < sizeof(ControlSumType); i++){
        controlSumChar[i] = tData.data[currentInd];
        currentInd++;
    }

    ControlSumType controlSum = *(ControlSumType*)controlSumChar;
    if(othersCount % sizeof(SymbolType) != 0){
        throw new Exceptions::MessageParsingException(System::String("message size doesn't multiple to size of SymbolType"), Exceptions::MessageParsingExceptionId_InvalidMessageId);
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
        throw new Exceptions::MessageParsingException(System::String("control sum doesn't match to real sum"), Exceptions::MessageParsingExceptionId_ControlSumMismatch);
    }

    Crypto::lastMessageIds->append(result.id);

    return result;
}

Message Crypto::parse_data(System::String data){
    try {
        return parse_data(data.data, data.size);
    } catch (Exceptions::Exception* exception) {
        throw exception;
    }
}

System::String Crypto::wrap_message(Message mes){
    unsigned int size = (sizeof(MessageIdType) +
                         sizeof (MessageTypeType) +
                         sizeof (ControlSumType)) +
                         mes.size * sizeof (SymbolType) +
                         sizeof (mes.partIndex) +
                         sizeof(mes.partsCount);

    System::String resultData = System::String(new char[size], size);

    unsigned int currentInd = 0;
    for(unsigned int i = 0; i < sizeof(MessageIdType); i++){
        resultData.data[currentInd] = ((char*)(&(mes.id)))[i];
        currentInd++;
    }

    for(unsigned int i = 0; i < sizeof(MessageTypeType); i++){
        resultData.data[currentInd] = ((char*)(&(mes.type)))[i];
        currentInd++;
    }

    for(unsigned int i = 0; i < sizeof(mes.partIndex); i++){
        resultData.data[currentInd] = ((char*)(&(mes.partIndex)))[i];
        currentInd++;
    }

    for(unsigned int i = 0; i < sizeof(mes.partsCount); i++){
        resultData.data[currentInd] = ((char*)(&(mes.partsCount)))[i];
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
        resultData.data[sizeof(MessageIdType) + sizeof(MessageTypeType) + sizeof(mes.partIndex) + sizeof (mes.partsCount) + i] = ((char*)&controlSum)[i];
    }

    System::String result;
    try {
        result = Crypto::_encrypt(resultData);
    } catch (Exceptions::Exception* exception) {
        throw exception;
    }

    return result;
}

bool Crypto::_message_id_ok(MessageIdType id)
{
    return/*(!(lastMessageIds->contains(id)) || id == -1)*/ true;
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
