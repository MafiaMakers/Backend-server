package Mafia.Events;

public abstract class MafiaEventListener {
    public MafiaEventListener(){
        EventsManager.add_listener(this);
    }

    public abstract void on_event(MafiaEvent event);
}
