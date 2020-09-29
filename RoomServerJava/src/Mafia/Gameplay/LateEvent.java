package Mafia.Gameplay;

import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;
import Mafia.Gameplay.Roles.IRoutine;
import java.lang.Boolean;

//!\brief Базовый класс отложенных игровых событий
public class LateEvent{
    //!\brief Рутина, которая должна запускаться при событии
    protected IRoutine onActionRoutine;
    //!\brief Рутина, которая должна запускаться для проверки истинности события (произошло ли оно).
    //!Такая рутина должна возвращать java.lang.Boolean
    protected IRoutine conditionRoutine;

    //!\brief Основной конструктор. Имена параметров совпадают с полями класса
    public LateEvent(IRoutine onActionRoutine, IRoutine conditionRoutine){
        this.onActionRoutine = onActionRoutine;
        this.conditionRoutine = conditionRoutine;
    }

    //!\brief Конструктор, который задает нулевую проверку (возвращающую только false)
    public LateEvent(IRoutine onActionRoutine){
        this(onActionRoutine, null);
    }

    //!\brief Переменная, отвечающая за то, произошло ли уже событие
    protected boolean finished = false;

    //!\brief Проверка на то, произошло ли событие
    public boolean check_condition() throws MafiaException{
        if(conditionRoutine != null){
            Object result = conditionRoutine.run();
            if(result.getClass().isInstance(Boolean.class)){
                return ((Boolean)result).booleanValue();
            } else {
                throw MafiaException.generate(ExceptionId.RoutineException, "INVALID CONDITION ROUTINE!!!");
            }
        } else{
            return false;
        }
    }

    //!\brief Метод, который вызывается при событии
    public Object on_action(){
        if(onActionRoutine != null){
            return onActionRoutine.run();
        } else{
            return null;
        }
    }
}
