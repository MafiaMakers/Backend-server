package Mafia.Exceptions;
/*!\brief Базовый класс исключений мафии (аналогичен плюсовскому Mafia::Exceptions::Exception)
 */
public class MafiaException extends Exception {
    //!ID исключения (по сути его тип)
    private ExceptionId id;

    //!\brief Геттер для \ref Mafia.Exceptions.MafiaException.id
    public ExceptionId get_id(){
        return id;
    }

    //!\brief Основной конструктор (приватныЙ!!!). Он нужен лишь для наследников.
    protected MafiaException(ExceptionId id, String data) {
        super(data);
        this.id = id;
    }

    /*!\brief Фабричный метод для исключений (создает объекты классов-наследников)
     *\param id ID исключения
     *\param data Текст, который следует вывести
    */
    public static MafiaException generate(ExceptionId id, String data){
        if(id == ExceptionId.MessageParsingException){
            return new NetworkException(id, data);
        }

        return new MafiaException(id, data);
    }

    //!\brief Вывод исключения в консоль
    public void show(){
        System.out.println("Exception!\nID: " + String.valueOf(id) + "\ndata: " + getMessage());
    }
}
