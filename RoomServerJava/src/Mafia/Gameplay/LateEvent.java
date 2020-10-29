package Mafia.Gameplay;

import Mafia.Events.MafiaEvent;
import Mafia.Events.MafiaEventListener;
import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;
import Mafia.Gameplay.Roles.IRoutine;
import java.lang.Boolean;

//!\brief Базовый класс отложенных игровых событий
public abstract class LateEvent extends MafiaEventListener {
    //! \brief Рутина, которая должна отработать по происшествии необходимого события
    IRoutine routine;

    //!\brief Основной конструктор
    //! \param routine Рутина, которую стоит вызвать при событии
    public LateEvent(IRoutine routine){
        setRoutine(routine);
    }

    //! \brief Функция, задающая рутину, которая будет вызываться событием
    //! \param routine Рутина
    public void setRoutine(IRoutine routine){
        this.routine = routine;
    }

    //!\brief Переменная, отвечающая за то, произошло ли уже событие
    protected boolean finished = false;

    //! \brief Абстрактный метод, который проверяет, то ли произошло событие или не то
    abstract protected boolean check_needed_event(GameEvent event);


    /*! \brief Наследованный от слушателя метод, который принимает событие и проверяет его на то, нужное ли событие
    произошло (вызывать ли рутину) - использует метод \ref check_needed_event. Если это то событие, то запускает рутину
        \param event Событие, которое произошло
     */
    @Override
    public void on_event(MafiaEvent event) {
        if(event instanceof GameEvent){
            if(check_needed_event((GameEvent)event)){
                routine.run();
            }
        }
    }
}
