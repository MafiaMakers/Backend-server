package Mafia.Gameplay.Roles;

//!\brief Основной интерфейс рутин. Суть такая: рутины - объекты, которые могут выполнять работу
//!(в java нет указателей на функции).
public interface IRoutine {
    //!\brief Как раз функция, запускающая рутину.
    Object run();
}
