package Mafia.Gameplay.Roles;
import java.util.Queue;

public class Role {
    private IRoutine[] defaultRoutines;
    private IRoutine[] currentRoutines;

    private Queue<RoleRoutineChangingRoundDelayedEvent> comingEvents;

    public void reset_routine(IRoutine toChange, int id){
        if(0 <= id && id < currentRoutines.length){
            currentRoutines[id] = toChange;
        }
    }
}
