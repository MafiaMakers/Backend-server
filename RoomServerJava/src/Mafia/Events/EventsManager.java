package Mafia.Events;

import Mafia.Network.MessageReceived;

import java.lang.ref.WeakReference;
import java.util.Collection;
import java.util.HashSet;

public class EventsManager {
    static {
        EventsManagerReconstructor r = new EventsManagerReconstructor();
    }

    private static Collection<WeakReference<MafiaEventListener>> listeners = new HashSet<WeakReference<MafiaEventListener>>();

    public static void register_event(MafiaEvent event){
        //System.out.println("Event registered");
        for (WeakReference<MafiaEventListener> listener : listeners){
            if(listener != null){
                listener.get().on_event(event);
            }
        }
    }

    public static void add_listener(MafiaEventListener listener){
        listeners.add(new WeakReference<MafiaEventListener>(listener));
    }

    public static void restructure(){
        Collection<WeakReference<MafiaEventListener>> n_listeners = new HashSet<WeakReference<MafiaEventListener>>();
        for(WeakReference<MafiaEventListener> listener : listeners){
            if(listener.get() != null){
                n_listeners.add(listener);
            }
        }

        listeners = n_listeners;
    }
}
