package Mafia.Gameplay.Roles;
import Mafia.Network.ClientInfo;

import java.util.ArrayList;
import java.awt.event.ActionListener;

//!\brief Базовый класс роли, отвечающий за все общие процессы для всех ролей.
public class Role implements ActionListener{
    //!\brief Список рутин, которые выполняются на каждой стадии игры (например, рутина, которая выполняется ночью, днем и т.п.)
    //!Но это список рутин, стандартных для этой роли.
    private IRoutine[] defaultRoutines;
    //!\brief Список рутин, которые актуальны сейчас (если какая-то стандартная рутина была удалена)
    private IRoutine[] currentRoutines;

    private RoleType roleType;

    private ClientInfo user;

    //!\brief Список событий по удалению и замене рутин. Например, если ночная бабочка на одну ночь изменила рутину этого игрока,
    //!он сразу создает себе событие на будущее, которое возвращает стандартную рутину к следующей ночи
    private ArrayList<RoleRoutineChangingRoundDelayedEvent> comingEvents;

    public RoleType getRoleType(){
        return roleType;
    }

    @Override
    public void actionPerformed(java.awt.event.ActionEvent actionEvent){

    }
    /*!\brief Функция жесткой смены рутины (используется событиями, возвращающими или изменяющими рутины
     *!\param toChange Новая рутина, которую следует поставить
     *!\param id ID рутины, которую следует заменить
     */
    public void reset_routine(IRoutine toChange, int id){
        if(0 <= id && id < currentRoutines.length){
            currentRoutines[id] = toChange;
        }
    }
}
