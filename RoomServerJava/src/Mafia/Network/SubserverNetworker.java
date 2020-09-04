package Mafia.Network;

//!\brief Основной класс, отвечающий за работу с сетью
public class SubserverNetworker {
    static {
        //Загрузка .so файла с определением нативных функций
        System.load(MessageJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") +
                "Mafia\\Network\\CPP_LIB\\SubserverNetworker.so");
    }
    //!\brief Указатель на объект Mafia::Network::MainServerNetworker, который будет работать
    private static long networkerPointer = 0;
    //!\brief Функция инициализации
    //!\param port Порт, на котором следует инициализировать субсервер
    public static native void init(int port);
    //!\brief Функция отправки сообщения
    //!\param messageJ сообщение, которое следует отправить
    public native static int sendMessage(MessageJ messageJ);
    /*!\brief Функция обработки полученных данных (просто сообщения из инета)
     *!\param data Данные, полученные из сети
     *!\param size Размер полученного сообщения
     *!\param ip IP, с которого было получено сообщение
     *!\param port Порт, с которого было отправлено сообщение
    */
    public static native void processReceivedBytes(byte[] data, int size, int ip, int port);

    //!\brief Функция обработки готового хорошего декодированного сообщения
    //!\param messageJ Сообщение, которое собрал MainServerNetworker
    public static void process_message(MessageJ messageJ){
        //System.out.println("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");
        //System.out.println("Received message from: " + String.valueOf(messageJ.Getclient().Getip()) + ":" + String.valueOf(messageJ.Getclient().Getport()));
        //System.out.println("Message type: " + String.valueOf(messageJ.Gettype()));
        System.out.println("ID: " + String.valueOf(messageJ.Getid()));
        //System.out.println("Data: " + String.valueOf(String.valueOf(messageJ.Getdata())));
        System.out.println("OK\nOK\nOK");
    }
}
