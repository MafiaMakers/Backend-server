package Mafia.Gameplay;
import Mafia.General.Events.MafiaEvent;


//!\brief Основной класс игровых событий
public class GameEvent extends MafiaEvent {
    //!\brief Основной конструктор. Совпадает с ActionEvent(Object, int, String)
    public GameEvent(Object source){
        super(source);
    }
}
