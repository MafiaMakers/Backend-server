package Mafia.Gameplay.Roles;

public class RoleRoutineChangingRoundDelayedEvent extends RoundDelayedEvent{
    private IRoutine toChange;
    private Role object;
    private int id;

    public RoleRoutineChangingRoundDelayedEvent(IRoutine conditionEvent, int id, IRoutine toChange, Role object){
        super(null, conditionEvent);
        this.id = id;
        this.object = object;
        this.toChange = toChange;
    }

    @Override
    public Object on_action() {
        object.reset_routine(toChange, id);
        return super.on_action();
    }
}
