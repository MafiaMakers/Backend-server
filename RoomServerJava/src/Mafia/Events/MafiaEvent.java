package Mafia.Events;

import Mafia.Network.MessageReceived;

public class MafiaEvent {
    private Object source;
    private String data;
    private int id;

    public MafiaEvent(){
        source = null;
        data = "";
        id = -1;
    }

    public MafiaEvent(Object source, String data, int id){
        this.source = source;
        this.data = data;
        this.id = id;
    }

    public void occur(){
        EventsManager.register_event(this);
    }

    public Object get_source(){
        return source;
    }

    public String get_data(){
        return data;
    }

    public int get_id(){
        return id;
    }


}
