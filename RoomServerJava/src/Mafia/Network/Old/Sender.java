package Mafia.Network.General;

public class Sender extends Thread {
    //!\brief Размер буфера сообщений
    protected static final int BUFFER_SIZE = 256;
    //!\brief Размер инта))
    protected static final int INT_SIZE = 4;
    //!\brief Сокет
    protected Object socket;
    //!\brief Проверка на то, инициализирован ли сокет
    protected boolean initialized = false;
    //!\brief Класс является синглтоном, так что это указатель на instance
    protected static Sender instance = null;
    //!\brief Проверка на то, работает ли сейчас сокет
    protected boolean running = true;

    //!\brief Метод, завершающий работу сокета
    public void terminate(){
        running = false;
        stop();
        instance = null;
    }

    //!\brief Приватный конструктор (пользователь должен обращаться лишь к статическим методам)
    //!\param port Порт, на котором следует создавать сокет
    protected Sender(int port){
        /*if(instance == null){
            instance = this;
        } else {
            throw new Exception("Message sender already has instance");
        }*/

        initialized = true;
        running = true;

        //System.out.println("Started");
    }

    @Override
    //!\brief Метод основной работы (получения сообщений) наследован от Thread
    public void run(){
        System.out.println("THIS IS SENDER");
    }

    //!\brief Функция отправки сообщения (приватная и не статическая!)
    protected void send_message(byte[] buffer, int size, int ip, int port){
        System.out.println("THIS IS SENDER SEND!!!");
    }
}
