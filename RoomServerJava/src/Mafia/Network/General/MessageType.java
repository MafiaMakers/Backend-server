package Mafia.Network.General;

import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;

//! \brief Перечисление всех типов сообщений
public enum MessageType {
    // Тут пока только дичь тестовая. Потом добавим норм сообщений
    First(0), Text (1), TestText (2), Last(3);

    private int intType;

    //! \brief Т.к. перечисление в java не так же работает как в c++ то есть явного приведения к int нет,
    //! приходится делать небольшой костыль в виде конструктора, принимающего int и функции, возвращающей int
    //! Это, собственно, конструктор. Но он обязательно приватный, поэтому есть еще функция, которая вызывает этот приватный конструктор )))
    MessageType(int idType){
        intType = idType;
    }

    //! \brief Функция, которая генерирует тип сообщения по числу. Если такого типа нет, то выкидывает исключение.
    //! Если же есть, то возвращает соответствующий тип
    public static MessageType from_int(int source) throws MafiaException{
        if(Last.getType() > source && source > First.getType()){
            return MessageType.values()[source];
        }

        throw MafiaException.generate(ExceptionId.MessageTypeException, "Unknown message type");
    }

    //! \brief Возвращает тип сообщения в виде int у данного типа.
    public int getType(){
        return intType;
    }

}
