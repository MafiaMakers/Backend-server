package Mafia.Network.General;

import Mafia.Exceptions.MafiaException;
import Mafia.Network.*;
import com.fasterxml.jackson.databind.JsonNode;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;

//! \brief Класс-отправитель сообщений. Используется как на сервере, так и на клиенте. Работает многопоточно
public class TCPSender extends Thread{
    //! \brief Порт сервера, к которому подключаемся
    private int serverPort;

    //! \brief Поток данных, в который пишутся данные для отправки
    private DataOutputStream out;
    //! \brief Поток данных, из которого читаются данные
    private DataInputStream in;
    //! \brief Сокет
    private Socket socket;

    //! \brief Переменная, показывающая, удалось ли создать объект-отправитель
    private boolean initialized;
    //! \brief Переменная для остановки объекта
    private boolean running;

    //! \brief Основной конструктор для клиента. Пробует подключиться к серверу и выбрасывает исключение, если не удалось
    public TCPSender(int serverPort) throws Exception {
        this.serverPort = serverPort;
        try {
            socket = new Socket(NetworkConfigs.get_server_ip(), serverPort);
            out = new DataOutputStream(((Socket)socket).getOutputStream());
            in = new DataInputStream(((Socket)socket).getInputStream());
            running = true;
            start();
        } catch (SocketException e){
            System.out.println(e.getMessage());
            initialized = false;
        }
    }

    //! \brief Основной конструктор для сервера. Подключается к уже существующему и соединенному с клиентом сокету
    //! и далее общается по нему. Выбрасывает исключение, если не удалось открыть потоки данных для сокетов
    public TCPSender(Socket sock) throws Exception{
        try{

            socket = sock;
            out = new DataOutputStream(((Socket)socket).getOutputStream());
            in = new DataInputStream(((Socket)socket).getInputStream());
            running = true;
            start();
        } catch (SocketException ex){
            System.out.println(ex.getMessage());
            initialized = false;
        }

    }

    @Override
    //!\brief Метод основной работы (получения сообщений) наследован от Thread
    public void run(){
        while (running && !((Socket)socket).isOutputShutdown()){
            try {
                byte[] message = new byte[NetworkConfigs.get_buffer_size()];
                int size = in.read(message);

                String ips[] = NetworkConfigs.get_server_ip().split(".");
                byte[] ip = new byte[4];
                for (int i = 0; i < ips.length; i++){
                    ip[i] = (byte)(int)Integer.parseInt(ips[i]);
                }

                String messageStr = new String(message, 0, size, StandardCharsets.UTF_8);

                MessageTCP mes = new MessageTCP();
                JsonNode mesData = MessageParser.parse(messageStr);
                mes.id = MessageType.from_int(mesData.get("id").asInt());

                mes.sender = new Client(((Socket) socket).getInetAddress().getAddress(), ((Socket) socket).getPort());
                mes.data = mesData.path("data");
                //running = false;
                MessageReceived m_event = new MessageReceived(mes, this);
                m_event.occur();
            } catch (IOException ex){
                System.out.print("IOException: ");
                System.out.println(ex.getMessage());
                ex.printStackTrace();
            } catch (MafiaException ex){
                System.out.print("MafiaException: ");
                System.out.println(ex.getMessage());
                ex.printStackTrace();
            }
        }
        System.out.println("Socket finished its work");
    }

    //! \brief Функция для сервера. Проверяет, с тем ли клиентом соединен этот сокет.
    //! \param other Клиент, с которым сравниваем сокет
    //! \returns Если клиент-аргумент и клиент, с которым соединен сокет, совпадают, возвращает true. Иначе false
    public boolean client_matches(Client other){
        if(socket == null){
            return false;
        }
        for (int i = 0; i < 4; i++){
            if(other.ip[i] != socket.getInetAddress().getAddress()[i]){
                return false;
            }
        }
        return (other.port == socket.getPort());
    }

    //! \brief Основная функция отправки сообщения
    //! \param message Сообщение, которое отправляем
    public void send_message(MessageTCP message){
        byte[] bytes = MessageParser.wrap_message(message).getBytes();
        //System.out.println(bytes.length);
        send_message(bytes);
    }

    //!\brief Функция отправки сообщения (приватная). Отправляет уже готовый набор байтов
    private void send_message(byte[] buffer){
        try {
            String res = new String(buffer, 0, buffer.length, StandardCharsets.UTF_8);
            System.out.println("Message sent: " + res);
            out.write(buffer);
            out.flush();
            //System.out.println("Message sent: " + res);
        } catch (IOException ex){
            System.out.print("IOException: ");
            System.out.println(ex.getMessage());
        }
    }
}
