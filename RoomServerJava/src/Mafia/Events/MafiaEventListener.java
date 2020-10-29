package Mafia.Events;

//! \brief Основной абстрактный класс слушателя события. Все солушатели должны наследоваться от него.
public abstract class MafiaEventListener {
    //! \brief Основной конструктор. Регистрирует себя в менеджере событий.
    public MafiaEventListener(){
        EventsManager.add_listener(this);
    }

    //! \brief Функция, которая должна быть переопределена. Функция обработки события.
    //! \param event Событие, которое произошло
    public abstract void on_event(MafiaEvent event);
}
