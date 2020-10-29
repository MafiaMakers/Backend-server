package Mafia.Network.General;
import Mafia.Events.MafiaEvent;

import java.awt.event.ActionEvent;

//! \brief Базовый класс для событий сети
public class NetworkEvent extends MafiaEvent {

    public NetworkEvent(){
        super();
    }

    public NetworkEvent(Object source){
        super(source);
    }

}
