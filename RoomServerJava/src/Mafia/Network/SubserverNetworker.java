package Mafia.Network;

import Mafia.Events.MafiaEvent;
import Mafia.Events.MafiaEventListener;
import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;
import Mafia.Gameplay.GameEvent;

import java.awt.event.ActionListener;
import java.net.ServerSocket;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedList;
import java.awt.event.ActionEvent;

//!\brief Основной класс, отвечающий за работу с сетью
public class SubserverNetworker extends MafiaEventListener {
    private Set<TCPSender> tcpSenders;
    ServerSocket tcpSocket;
    private UDPSender sender;
    private int TCPPort;

    public SubserverNetworker(int TCPPort, int UDPPort){

        this.TCPPort = TCPPort;
        tcpSenders = new HashSet<TCPSender>();
        try {
            sender = new UDPSender(UDPPort);
            tcpSocket = new ServerSocket(TCPPort);
            tcpSocket.setReuseAddress(true);
            tcpSenders.add(new TCPSender(tcpSocket));
        } catch (Exception ex){
            System.out.println(ex.getMessage());
        }
    }

    public void send_message(MessageJ message){
        try {
            if (NetworkConfigs.udpOrTcpMessage(MessageType.from_int(message.Gettype()))) {
                sender.send_message(message);
            } else{
                ClientJ receiver = message.Getclient();
                boolean found = false;
                for (TCPSender sndr : tcpSenders){
                    if(sndr.client_matches(receiver)){
                        sndr.send_message(message);
                        found = true;
                        break;
                    }
                }

                if(!found){
                    throw MafiaException.generate(ExceptionId.ClientMismatchException, "Client not found!!!");
                }
            }
        } catch(MafiaException ex){
            System.out.println(ex.getMessage());
        }
    }

    /*static {
        //Загрузка .so файла с определением нативных функций
        System.load(MessageJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") +
                "Mafia\\Network\\CPP_LIB\\SubserverNetworker.so");
    }*/
    //!\brief Указатель на объект Mafia::Network::MainServerNetworker, который будет работать
    //private static long networkerPointer = 0;
    //!\brief Функция инициализации
    //!\param port Порт, на котором следует инициализировать субсервер
    //public static native void init(int port);
    //!\brief Функция отправки сообщения
    //!\param messageJ сообщение, которое следует отправить
    //public native static int sendMessage(MessageJ messageJ);
    /*!\brief Функция обработки полученных данных (просто сообщения из инета)
     *!\param data Данные, полученные из сети
     *!\param size Размер полученного сообщения
     *!\param ip IP, с которого было получено сообщение
     *!\param port Порт, с которого было отправлено сообщение
    */
    //public static native void processReceivedBytes(byte[] data, int size, int ip, int port);

    public void on_event(MafiaEvent event){
        //System.out.println("on_event in networker");
        if(event instanceof NetworkEvent){
            if(event instanceof ClientConnected){
                try {
                    System.out.print("Client connected: ");
                    System.out.print(((ClientConnected) event).client.Getip());
                    System.out.print(" ");
                    System.out.println(((ClientConnected) event).client.Getport());
                    tcpSenders.add(new TCPSender(tcpSocket));
                } catch (Exception ex){
                    System.out.println(ex.getMessage());
                }
            }
            if(event instanceof MessageReceived){
                //System.out.println("Message received");
                process_message(((MessageReceived) event).get_message());
            }
            if(event instanceof ClientLeaved){
                tcpSenders.remove((TCPSender) event.get_source());
                /*for (TCPSender sndr : tcpSenders){
                    if(sndr == event.get_source()){
                        ;
                        break;
                    }
                }*/
            }
        }
    }

    //!\brief Функция обработки готового хорошего декодированного сообщения
    //!\param messageJ Сообщение, которое собрал MainServerNetworker
    private static void process_message(MessageJ messageJ){
        //System.out.println("MESSAGE RECEIVED!!! PROCESSING!!!");
        System.out.println(messageJ);
        //System.out.println("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");
        System.out.println("Received message from: " + messageJ.Getclient().Getip() + ":" +
                String.valueOf(messageJ.Getclient().Getport()));
        System.out.println("Message type: " + messageJ.Gettype());
        System.out.println("ID: " + messageJ.Getid());
        System.out.println("Data: " + String.valueOf(messageJ.Getdata()));
    }
}
