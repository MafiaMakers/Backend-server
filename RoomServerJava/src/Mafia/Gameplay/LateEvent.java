package Mafia.Gameplay;

import Mafia.Exceptions.MafiaException;
import Mafia.Gameplay.Roles.IRoutine;
import java.lang.Boolean;

public abstract class LateEvent {
    private IRoutine onActionRoutine;
    private IRoutine conditionRoutine;

    public LateEvent(IRoutine onActionRoutine, IRoutine conditionRoutine){
        this.onActionRoutine = onActionRoutine;
        this.conditionRoutine = conditionRoutine;
    }

    public LateEvent(IRoutine onActionRoutine){
        this(onActionRoutine, null);
    }

    private boolean finished = false;

    public boolean check_condition() throws MafiaException{
        if(conditionRoutine != null){
            Object result = conditionRoutine.run();
            if(result.getClass().isInstance(Boolean.class)){
                return ((Boolean)result).booleanValue();
            } else {
                throw MafiaException.generate(1, "INVALID CONDITION ROUTINE!!!");
            }
        } else{
            return false;
        }
    }

    public Object on_action(){
        if(onActionRoutine != null){
            return onActionRoutine.run();
        } else{
            return null;
        }
    }
}
