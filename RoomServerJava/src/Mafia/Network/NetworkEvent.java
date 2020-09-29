package Mafia.Network;
import Mafia.Events.MafiaEvent;

import java.awt.event.ActionEvent;

public class NetworkEvent extends MafiaEvent {

    public NetworkEvent(){
        super();
    }

    public NetworkEvent(Object source, String data, int id){
        super(source, data, id);
    }
    //!\brief Основной конструктор. Совпадает с ActionEvent(Object, int, String)
    //public NetworkEvent(Object source, int id, String command){
        //super(source, id, command);
    //}
}
