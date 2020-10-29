package Mafia.Network.Old;

import Mafia.Network.General.Sender;

import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.util.ArrayList;

//!\brief Класс, который занимается отправкой и получением пакетов по сети
public class UDPSender extends Sender {

    private static final int MAX_MESSAGE_SIZE = 512;

    private ArrayList<SeparatedMessage> notFullMessages;

    //!\brief Приватный конструктор (пользователь должен обращаться лишь к статическим методам)
    //!\param port Порт, на котором следует создавать сокет
    public UDPSender(int port) throws Exception{
        super(port);
        notFullMessages = new ArrayList<SeparatedMessage>();
        try {
            socket = new DatagramSocket(port);
            start();
        } catch (SocketException e){
            System.out.println(e.getMessage());
            initialized = false;
        }
    }

    @Override
    //!\brief Метод основной работы (получения сообщений) наследован от Thread
    public void run(){
        if(initialized){
            while (running){
                byte[] buf = new byte[BUFFER_SIZE];
                //System.out.println("Receiving");
                DatagramPacket packet = new DatagramPacket(buf, BUFFER_SIZE);

                try {
                    ((DatagramSocket)socket).receive(packet);
                } catch (IOException ex){
                    System.out.println(ex.getMessage());
                    continue;
                }
                //System.out.println("Message received!!!");
                InetAddress address = packet.getAddress();
                byte[] addressBytes = address.getAddress();
                int ip = 0;
                for (int i = 0; i < addressBytes.length; i++){
                    ip += ((int)addressBytes[i]) << 8*(3-i);
                }

                int port = packet.getPort();

                //packet = new DatagramPacket(buf, BUFFER_SIZE, address, port);

                //String received = new String(packet.getData(), packet.getLength());
                //System.out.println("Starting processing message");

                MessageUDP message = UDPMessageParser.parse(packet.getData(), ip, port);

                //MessageJ mes = new MessageJ();
                //mes.Setdata("qwertyu".toCharArray());
                //System.out.println(mes.Getdata());

                //message.Setdata("qwertyu".toCharArray());

                boolean found = false;

                for(int i = 0; i < notFullMessages.size(); i++){
                    if(notFullMessages.get(i).matches(message)){
                        found = true;
                        notFullMessages.get(i).add(message);
                        process(i);
                        break;
                    }
                }

                if(!found){
                    notFullMessages.add(new SeparatedMessage(message));
                    //notFullMessages.get(notFullMessages.size() - 1).add(message);
                    process(notFullMessages.size() - 1);
                }
            }
            ((DatagramSocket)socket).close();
        } else{
            System.out.println("Socket is not initialized, cannot run server");
        }
    }

    private void process(int index){
        //System.out.println("PROCESSING");
        if(notFullMessages.get(index).is_complete()){
            //System.out.println("in");
            try {
                MessageJ whole = notFullMessages.get(index).construct_whole();
                //System.out.println(whole.Getdata());
                UDPMessageReceived messageReceived = new UDPMessageReceived(whole, this);
                messageReceived.occur();
                notFullMessages.remove(index);
            } catch (Exception ex){
                System.out.println(ex.getMessage());
            }
        }
        //System.out.println("PROCESSING FINISHED");
    }

    //!\brief Функция отправки сообщения (приватная и не статическая!)
    protected void send_message(byte[] buffer, int size, int ip, int port){
        //System.out.println("Asked to send message by UDP:");
        //System.out.println(String.valueOf(buffer));
        //System.out.println(ip);
        //System.out.println(port);
        //System.out.println(String.valueOf(size));
        byte[] bytes = ByteBuffer.allocate(INT_SIZE).putInt(ip).array();

        try{
            InetAddress address = InetAddress.getByAddress(bytes);

            DatagramPacket packet = new DatagramPacket(buffer, size, address, port);

            ((DatagramSocket)socket).send(packet);
        } catch (IOException ex){
            System.out.println(ex.getMessage());
        }
    }

    public void send_message(MessageJ message){
        //System.out.println("Sending message");
        int count = message.Getdata().length / MAX_MESSAGE_SIZE + 1;

        String data = String.valueOf(message.Getdata());

        for (int i = 0; i < count; i++){
            int endIndex = i == count - 1 ? data.length() : (i + 1) * MAX_MESSAGE_SIZE;
            MessageJ current = new MessageJ(message.Getid(), count, i, message.Gettype(), message.Getclient(),
                    data.substring(i * MAX_MESSAGE_SIZE, endIndex).toCharArray());

            byte[] buffer = UDPMessageParser.wrap(current);
            int size = buffer.length;
            int ip = message.Getclient().Getip();
            int port = message.Getclient().Getport();

            send_message(buffer, size, ip, port);

        }
    }

}
