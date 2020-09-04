package Mafia.Gameplay.Roles;

import Mafia.Gameplay.ChangeRoundGameEvent;
import Mafia.Gameplay.GameStage;

import java.awt.event.ActionEvent;
//!\brief Событие, которое меняет рутины стадий игры у ролей
public class RoleRoutineChangingRoundDelayedEvent extends RoundDelayedEvent{
    //!\brief Рутина, на которую следует заменить старую
    private IRoutine toChange;
    //!\brief Объект роли, у которого следует изменять рутину
    private Role object;
    //!\brief ID рутины, которую следует заменить
    private int id;
    //!\brief Количество раундов, через которое следует проводить замену
    private int roundLeft;
    //!\brief Стадия игры, на которой следует производить замену
    private GameStage triggerStage;

    //!\brief Основной конструктор (количество раундов ожидания будет установлен в 1).
    //!Имена параметров совпадают с соответствующими именами полей
    public  RoleRoutineChangingRoundDelayedEvent(GameStage onStage, int id, IRoutine toChange, Role object){
        this(onStage, id, toChange, object, 1);
    }

    //!\brief Основной конструктор с указанием количества раундов ожидания
    public RoleRoutineChangingRoundDelayedEvent(GameStage triggerStage, int id, IRoutine toChange, Role object, int roundsToSkip){
        super(null, null);
        this.id = id;
        this.object = object;
        this.toChange = toChange;
        this.roundLeft = roundsToSkip;
        this.triggerStage = triggerStage;
    }

    //!\brief Метод, наследованный из ActionListener.
    //!\param Событие, которое произишло
    public void actionPerformed(ChangeRoundGameEvent actionEvent) {
        if(!finished) {
            if (actionEvent.is_stage(triggerStage)) {
                roundLeft--;
            }
            if (roundLeft <= 0) {
                finished = true;
                on_action();
            }
        }
    }

    @Override
    //!\brief метод, вызываемый при срабатывании этого события!
    public Object on_action() {
        object.reset_routine(toChange, id);
        return super.on_action();
    }
}
