package Mafia.Gameplay;
import java.awt.event.ActionEvent;

//!\brief Основной класс игровых событий
public class GameEvent extends ActionEvent{
    //!\brief Основной конструктор. Совпадает с ActionEvent(Object, int, String)
    public GameEvent(Object source, int id, String command){
        super(source, id, command);
    }
}
