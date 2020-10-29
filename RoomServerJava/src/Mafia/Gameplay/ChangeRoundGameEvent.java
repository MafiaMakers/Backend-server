package Mafia.Gameplay;

//!\brief Событие смены стадии игры
public class ChangeRoundGameEvent extends GameEvent{
    //!\brief Новая стадия игры, которая наступила
    private GameStage stage;

    /*!\brief Основной конструктор
     *!\param source Объект, создающий это событие (чаще всего стоит указывать this)
     *!\param id ID события (если не знаете, что указать, укажите -1)
     *!\param command Строка, которая характеризует это событие
     *!\param stage Стадия игры, которая наступила
     */
    public ChangeRoundGameEvent(Object source, GameStage stage){
        super(source);
        this.stage = stage;
    }

    //!\brief Проверка на то, равна ли наступившая стадия аргументу
    //!\param other Стадия, с которое следует сверить текущую стадию
    public boolean is_stage(GameStage other){
        return other.equals(stage);
    }
}
