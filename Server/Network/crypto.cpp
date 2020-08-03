#include "crypto.h"
#include "Exceptions/messageparsingexception.h"
#include "System/serializer.h"
#include "System/tuple.h"

using namespace Mafia;
using namespace Network;
System::LimitedQueue<MessageIdType>* Crypto::lastMessageIds = new System::LimitedQueue<MessageIdType>(rememberMessagesCount);
System::String Crypto::key = System::String();

void Crypto::set_key(System::String key){
    Crypto::key = key;
}

void Crypto::set_key(std::string key){
	Crypto::key = System::String(key);
}

void Crypto::set_key(char* key, int size){
    Crypto::key = System::String(key, size);
}

Message Crypto::parse_data(char *data, int size){
	//Дешифруем сообщение
	System::String tData = System::String();
    try {
        tData = Crypto::_decrypt(System::String(data, size));
    } catch (Exceptions::Exception* exception) {
        switch (exception->get_id()) {
        case Exceptions::MessageParsingExceptionId_ShortMessage:{
            exception->show();
			return Message();
            break;
        }
        default:{
            throw exception;
        }
        }
    }

	//Десериализуем в сообщение и контрольную сумму
	System::Tuple<Message, ControlSumType> result =
			System::Serializer::deserialize< System::Tuple< Message, ControlSumType > >( tData );

	//Считаем настоящую контрольную сумму
    ControlSumType realSum = 0;
    for(unsigned int i = 0; i < result.item1.size * sizeof(SymbolType); i++){
        realSum += (int)(char)((char*)result.item1.data)[i];
    }

    if(result.item2 != realSum){
		throw Exceptions::Exception::generate(System::String("control sum doesn't match to real sum " +
																	 QString::number(result.item2).toStdString() + " " +
																	 QString::number(realSum).toStdString()),
													  Exceptions::MessageParsingExceptionId_ControlSumMismatch);
    }

	//Для проверки id сообщений, добавляем id этого в список
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

	//Считаем контрольную сумму
    ControlSumType controlSum = 0;
    for(int i = 0; i < mes.size; i++){
        for(unsigned int j = 0; j < sizeof (SymbolType); j++){
			controlSum += (ControlSumType)( ( ( char* ) &mes.data[ i ] )[ j ] );
        }
    }

	//Сериализуем сообщение
	System::String data = System::String(
			System::Serializer::serialize< System::Tuple< Message, ControlSumType > >(
					System::Tuple< Message, ControlSumType >( mes, controlSum )
					)
			);

	//Шифруем сообщение
    System::String result;
    try {
		result = Crypto::_encrypt( data );
    } catch (Exceptions::Exception* exception) {
        throw exception;
    }

    return result;
}

bool Crypto::_message_id_ok(MessageIdType id)
{
	/* Даже не знаю, нужно ли здесь что-то...
	 * Вообще, эта функция была предназначена для того,
	 * чтобы проверять id сообщений на повторения...
	 * То есть чтобы не вышло так, что разные пользователи отправляют сообщения с одинаковыми id
	 * Но возможно, она вообще не нужна и ее стоит удалить
	*/
    return true;
}

System::String Crypto::_encrypt(System::String decrypted){
	//Ключ должен быть установлен
    if(Crypto::key.size == 0){
		throw Exceptions::Exception::generate(System::String("encryption failed! Key is not set"), Exceptions::MessageParsingExceptionId_NoneKey);
    }
	char * d;
	SAFE_NEW(d, char[decrypted.size]);
	System::String encrypted = System::String(d, decrypted.size);
	//Побитово прибавляем к строке символы ключа
    for(int i = 0; i < decrypted.size; i++){
		short num = ( short )decrypted.data[ i ] + ( short )Crypto::key.data[ i % Crypto::key.size ];
        if(num > 128){
            num -= 256;
        }
        if(num < -128){
            num += 256;
        }
		encrypted.data[i] = ( char )num;
    }

    return encrypted;
}

System::String Crypto::_decrypt(System::String encrypted){
	//Ключ должен быть установлен (и должен совпадать во всех клиентах)
    if(Crypto::key.size == 0){
		throw Exceptions::Exception::generate(System::String("decryption failed! Key is not set"), Exceptions::MessageParsingExceptionId_NoneKey);
    }

	//Побитово вычитаем из строки символы ключа
	char* d;
	SAFE_NEW(d, char[encrypted.size]);
	System::String decrypted = System::String(d, encrypted.size);
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
