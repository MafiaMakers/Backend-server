package Mafia.Network.Old;

import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;
import Mafia.Network.ClientConnected;
import Mafia.Network.General.*;
import Mafia.Network.General.MessageParser;


import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class TCPSender extends Sender {
    protected Socket client = null;

    protected DataOutputStream out;
    //!\brief Приватный конструктор (пользователь должен обращаться лишь к статическим методам)
    //!\param port Порт, на котором следует создавать сокет
    public TCPSender(ServerSocket sock) throws Exception{
        super(sock.getLocalPort());
        socket = sock;
        start();
    }

    @Override
    //!\brief Метод основной работы (получения сообщений) наследован от Thread
    public void run(){
        try{

            client = ((ServerSocket)socket).accept();
            ClientConnected event = new ClientConnected(parse_client(client), this);
            event.occur();

            DataInputStream in = new DataInputStream(client.getInputStream());
            out = new DataOutputStream(client.getOutputStream());

            if(initialized){
                while (running){
                    //System.out.println("Server is waiting for message");
                    //String recv = in.readAllBytes();
                    //System.out.println(recv.length());
                    byte[] message = new byte[NetworkConfigs.get_buffer_size()];
                    int size = in.read(message);//recv.getBytes();
                    //System.out.println(message.length);
                    //System.out.println("Message received: " + message);
                    //ClientJ currentClient = parse_client(client);
                    //SubserverNetworker.processReceivedBytes(message.getBytes(), message.length(), ip, port);

                    //System.out.println(message.length);
                    MessageTCP mes = new MessageTCP();
                    mes.data = MessageParser.parse(String.valueOf(message));
                    mes.sender.ip = client.getInetAddress().getAddress();
                    mes.sender.port = client.getPort();
                    //running = false;
                    MessageReceived m_event = new MessageReceived(mes, this);
                    m_event.occur();
                    //process(received, address.toString(), port);
                }
                ((DatagramSocket)socket).close();
            } else{
                System.out.println("Socket is not initialized, cannot run server");
            }
        } catch (IOException ex){
            System.out.println(ex.getMessage());
            terminate();
        }

    }

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

    public boolean client_matches(Client other){
        if(client == null){
            return false;
        }
        try {
            InetAddress address = InetAddress.getByAddress(other.ip);
            return (address == client.getInetAddress() && other.port == client.getPort());
        } catch (UnknownHostException ex){
            System.out.println(ex.getMessage());
            return false;
        }
    }

    public void send_message(MessageTCP message) throws MafiaException{
        if(client_matches(message.sender)) {
            send_message(message.Getdata().toString().getBytes(), message.Getdata().length, message.Getclient().Getip(), message.Getclient().Getport());
        } else {
            throw MafiaException.generate(ExceptionId.ClientMismatchException, "Wrong client!!!");
        }
    }

    //!\brief Функция отправки сообщения (приватная и не статическая!)
    protected void send_message(byte[] buffer, int size, int ip, int port){

        try{
                //System.out.println("Asked to send message by TCP:");
                //System.out.println(String.valueOf(buffer));
                //System.out.println(ip);
                //System.out.println(port);
                //System.out.println(String.valueOf(size));

                //String data = String.valueOf(buffer);
                out.write(buffer);
                out.flush();
        } catch (IOException ex){
            System.out.println(ex.getMessage());
        }
    }
}
