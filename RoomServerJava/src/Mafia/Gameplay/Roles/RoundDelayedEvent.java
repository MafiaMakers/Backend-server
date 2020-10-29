package Mafia.Gameplay.Roles;
import Mafia.Events.MafiaEvent;
import Mafia.Events.MafiaEventListener;
import Mafia.Gameplay.ChangeRoundGameEvent;
import Mafia.Gameplay.GameEvent;
import Mafia.Gameplay.GameStage;
import Mafia.Gameplay.LateEvent;

import java.awt.event.ActionListener;
//!\brief Класс для событий, которые как либо привязаны к стадиям игры
public class RoundDelayedEvent extends LateEvent {
    //! \brief Сколько раундов еще ждать
    private int roundLeft;
    //! \brief Стадия игры, на которой происходит смена
    private GameStage triggerStage;

    /*! \brief Основной конструктор.
        \param onActionRoutine рутина, которую следует вызвать по прошествии необходимого кол-ва раундов
        \param trigger Стадия игры, во время которой должна произойти смена
        \param roundsToWait Кол-во раундов, которые мы пропустим, прежде чем запускать событие
     */
    public RoundDelayedEvent(IRoutine onActionRoutine, GameStage trigger, int roundsToWait){
        super(onActionRoutine);
        roundLeft = roundsToWait;
        triggerStage = trigger;
    }


    //! \brief Функция, которая проверяет, пришло ли время запускать событие.
    //! Отсчитывает нужное количество раундов и затем запускает событие
    @Override
    protected boolean check_needed_event(GameEvent event){
        if(event instanceof ChangeRoundGameEvent){
            if(((ChangeRoundGameEvent) event).is_stage(triggerStage)){
                roundLeft--;
            }
        }

        if(roundLeft == 0){
            return true;
        }
        return false;
    }

}
