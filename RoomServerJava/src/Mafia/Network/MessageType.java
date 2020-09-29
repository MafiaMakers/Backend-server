package Mafia.Network;

import Mafia.Events.MafiaEvent;
import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;

public enum MessageType {
    First(0), TestUDPMessage (1), TestTCPMessage (2), Last(3);

    private int intType;

    MessageType(int idType){
        intType = idType;
    }

    public static MessageType from_int(int source) throws MafiaException{
        if(Last.getType() > source && source > First.getType()){
            return MessageType.values()[source];
        }

        throw MafiaException.generate(ExceptionId.MessageTypeException, "Unknown message type");
    }

    public int getType(){
        return intType;
    }

}
