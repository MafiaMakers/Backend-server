package Mafia.Network;

import Mafia.General.Network.Client;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;

//! \brief Класс, отвечающий за подключение новых клиентов. Создает события ClientConnected. Работает многопоточно
public class ClientConnecter extends Thread{
    //! \brief Серверный сокет, к которому будут подключаться клиенты
    private ServerSocket socket;
    //! \brief Переменная для остановки работы класса
    private boolean running;

    //! \brief Основной конструктор
    public ClientConnecter(ServerSocket socket){
        this.socket = socket;
        running = true;
        start();
    }

    //! \brief Остановка работы
    public void terminate(){
        running = false;
    }

    //! \brief Основная функция работы. При каждом новом подключении создает событие ClientConnected. Работает в отдельном потоке
    @Override
    public void run(){
        while (running){
            try {
                Socket client = socket.accept();
                ClientConnected event = new ClientConnected(parse_client(client), client, this);
                event.occur();
            } catch (IOException ex){
                ex.printStackTrace();
            }
        }
    }

    //! \brief Вспомогательная функция, которая из сокета получает данные клиента
    private static Client parse_client(Socket client){
        InetAddress address = client.getInetAddress();
        byte[] addressBytes = address.getAddress();
        int ip = 0;
        for (int i = 0; i < addressBytes.length; i++){
            ip += ((int)addressBytes[i]) << 8*(3-i);
        }

        int port = client.getPort();

        return new Client(ByteBuffer.allocate(4).putInt(ip).array(), port);
    }
}
