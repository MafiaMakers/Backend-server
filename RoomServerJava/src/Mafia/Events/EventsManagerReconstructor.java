package Mafia.Events;

import java.util.concurrent.ExecutionException;

public class EventsManagerReconstructor extends Thread {
    private static int delay = 5000;

    private boolean running = true;

    public EventsManagerReconstructor(){
        start();
    }

    public void terminate(){
        running = false;
        stop();
    }

    public void run(){
        while (running) {
            try {
                sleep(delay);
            } catch (Exception ex){
                System.out.println(ex.getMessage());
            }
            EventsManager.restructure();
        }
    }
}
