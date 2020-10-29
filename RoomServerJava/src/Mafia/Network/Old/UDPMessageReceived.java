package Mafia.Network.Old;

import Mafia.Events.MafiaEvent;

public class UDPMessageReceived extends MafiaEvent {
    protected MessageUDP message;

    public UDPMessageReceived(MessageUDP message, Object source){
        //System.out.println("MessageReceived event constructor");
        this.message = message;
        //System.out.println(message.Getdata());
    }

    public MessageUDP get_message(){
        return message;
    }
}
