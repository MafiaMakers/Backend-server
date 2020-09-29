package Mafia.Exceptions;

public class NetworkException extends MafiaException {
    //!\brief Основной конструктор (приватныЙ!!!). Он нужен лишь для наследников.
    protected NetworkException(ExceptionId id, String data) {
        super(id, data);
    }
}
