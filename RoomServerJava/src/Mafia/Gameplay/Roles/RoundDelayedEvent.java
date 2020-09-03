package Mafia.Gameplay.Roles;
import Mafia.Gameplay.LateEvent;

import java.awt.event.ActionListener;
public class RoundDelayedEvent extends LateEvent implements ActionListener{
    public RoundDelayedEvent(IRoutine onActionRoutine){
        this(onActionRoutine, null);
    }

    public RoundDelayedEvent(IRoutine onActionRoutine, IRoutine conditionRoutine){
        super(onActionRoutine, conditionRoutine);
    }

    @Override
    public void actionPerformed(java.awt.event.ActionEvent actionEvent){

    }

    @Override
    public boolean check_condition(){
        return false;
    }

    public Object on_action(){
        return null;
    }
}
