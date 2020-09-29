package Mafia.Network;

import java.io.*;
import java.net.Inet4Address;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.nio.charset.StandardCharsets;

public class TCPClient extends Sender {
    private static final String SERVER_IP = "127.0.0.1";
    private int serverPort;

    private DataOutputStream out;
    private DataInputStream in;


    public TCPClient(int serverPort, int port) throws Exception {
        super(port);
        this.serverPort = serverPort;
        try {
            socket = new Socket("localhost", serverPort);
            out = new DataOutputStream(((Socket)socket).getOutputStream());
            in = new DataInputStream(((Socket)socket).getInputStream());
            start();
        } catch (SocketException e){
            System.out.println(e.getMessage());
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

                String ips[] = SERVER_IP.split(".");
                int ip = 0;
                for (int i = 0; i < ips.length; i++){
                    ip += (Integer.parseInt(ips[i])) << (ips.length - i - 1);
                }

                MessageJ mes = MessageParser.parse_message(message, size, ip, serverPort);
                //running = false;
                MessageReceived m_event = new MessageReceived(mes, this);
                m_event.occur();
            } catch (IOException ex){
                System.out.print("IOException: ");
                System.out.println(ex.getMessage());
            }
        }
    }

    public void send_message(MessageJ message){
        byte[] bytes = MessageParser.wrap_message(message);
        //System.out.println(bytes.length);
        send_message(bytes);
    }

    //!\brief Функция отправки сообщения (приватная и не статическая!)
    private void send_message(byte[] buffer){
        try {
            String res = new String(buffer, 0, buffer.length, StandardCharsets.UTF_8);
            out.write(buffer);
            out.flush();
            //System.out.println("Message sent: " + res);
        } catch (IOException ex){
            System.out.print("IOException: ");
            System.out.println(ex.getMessage());
        }
    }
}
