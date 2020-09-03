package Mafia.Gameplay;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class GameEvent extends ActionEvent{
    public GameEvent(Object source, int id, String command){
        super(source, id, command);
    }

}
