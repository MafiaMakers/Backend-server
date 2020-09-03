package Mafia.Network;

import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;

public class MessageSender extends Thread{

    private static final int BUFFER_SIZE = 256;
    private static final int INT_SIZE = 4;
    private DatagramSocket socket;
    private boolean initialized = false;
    private static MessageSender instance = null;
    private boolean running = true;

    public static void init(int port){
        try{
            System.out.println("Initializing message sender");
            MessageSender current = new MessageSender(port);
            System.out.println("Successfully finished");
        } catch (Exception ex){
            System.out.println("Error in initialization of server: " + ex.getMessage());
        }
    }

    public static void send(byte[] buffer, int size, int ip, int port){
        instance.send_message(buffer, size, ip, port);
    }

    public static void terminate(){
        instance.running = false;
        instance.stop();
        instance = null;
    }

    public MessageSender(int port) throws Exception{
        try {
            if(instance == null){
                instance = this;
            } else {
                throw new Exception("Message sender already has instance");
            }
            socket = new DatagramSocket(port);
            initialized = true;
            running = true;
            start();

            System.out.println("Started");
        } catch (SocketException e){
            System.out.println(e.getMessage());
            initialized = false;
        }
    }

    @Override
    public void run(){
        if(initialized){
            while (running){
                byte[] buf = new byte[BUFFER_SIZE];
                System.out.println("Receiving");
                DatagramPacket packet = new DatagramPacket(buf, BUFFER_SIZE);

                try {
                    socket.receive(packet);
                } catch (IOException ex){
                    System.out.println(ex.getMessage());
                    continue;
                }
                System.out.println("Message received!!!");
                InetAddress address = packet.getAddress();
                byte[] addressBytes = address.getAddress();
                int ip = 0;
                for (int i = 0; i < addressBytes.length; i++){
                    ip += ((int)addressBytes[i]) << 8*(3-i);
                }

                int port = packet.getPort();

                //packet = new DatagramPacket(buf, BUFFER_SIZE, address, port);

                //String received = new String(packet.getData(), packet.getLength());
                System.out.println("Starting processing message");
                SubserverNetworker.processReceivedBytes(packet.getData(), packet.getLength(), ip, port);
                running = false;
                //process(received, address.toString(), port);
            }
            socket.close();
        } else{
            System.out.println("Socket is not initialized, cannot run server");
        }
    }

    private void send_message(byte[] buffer, int size, int ip, int port){
        System.out.println("Asked to send message:");
        System.out.println(String.valueOf(buffer));
        System.out.println(ip);
        System.out.println(port);
        System.out.println(String.valueOf(size));
        byte[] bytes = ByteBuffer.allocate(INT_SIZE).putInt(ip).array();

        try{
            InetAddress address = InetAddress.getByAddress(bytes);

            DatagramPacket packet = new DatagramPacket(buffer, size, address, port);

            socket.send(packet);
        } catch (IOException ex){
            System.out.println(ex.getMessage());
        }
    }

}
