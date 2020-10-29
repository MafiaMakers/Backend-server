package Mafia.Gameplay.Roles;

import Mafia.Gameplay.ChangeRoundGameEvent;
import Mafia.Gameplay.GameStage;

import java.awt.event.ActionEvent;
//!\brief Событие, которое меняет рутины стадий игры у ролей
public class RoleRoutineChangingRoundDelayedEvent extends RoundDelayedEvent implements IRoutine{
    //!\brief Рутина, на которую следует заменить старую
    private IRoutine toChange;
    //!\brief Объект роли, у которого следует изменять рутину
    private Role object;
    //!\brief ID рутины, которую следует заменить
    private int id;

    //!\brief Основной конструктор (количество раундов ожидания будет установлен в 1).
    //! \param onStage Стадия игры, которая нам важна (на которой происходит событие)
    //! \param id id рутины в объекте роли
    //! \param toChange Рутина на замену - которую надо поставить
    //! \param role object Объект роли, в котором следует произвести изменения
    public  RoleRoutineChangingRoundDelayedEvent(GameStage onStage, int id, IRoutine toChange, Role object){
        this(onStage, id, toChange, object, 1);
    }

    //!\brief Основной конструктор с указанием количества раундов ожидания
    public RoleRoutineChangingRoundDelayedEvent(GameStage triggerStage, int id, IRoutine toChange, Role object, int roundsToSkip){
        super(null, triggerStage, roundsToSkip);
        this.id = id;
        this.object = object;
        this.toChange = toChange;

        setRoutine(this);
    }

    //! \brief Функция, заимствованная у \ref IRoutine. Заменяет рутину на объекте роли
    public Object run(){
        object.reset_routine(toChange, id);
        return null;
    }

}
