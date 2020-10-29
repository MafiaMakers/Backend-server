package Mafia.Network;

import Mafia.Events.MafiaEvent;
import Mafia.Events.MafiaEventListener;
import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;
import Mafia.Network.General.*;

import java.net.ServerSocket;
import java.util.Set;
import java.util.HashSet;

//!\brief Основной класс, отвечающий за работу с сетью
public class SubserverNetworker extends MafiaEventListener {
    //! \brief Список всех объектов-отправителей (1 объект на 1 клиента)
    private Set<TCPSender> tcpSenders;

    //! \brief Объект, подключающий новых клиентов
    private ClientConnecter connecter;

    //! \brief Основной серверный сокет
    ServerSocket tcpSocket;
    //! \brief Порт, на котором инициализирован сервер
    private int TCPPort;

    //! \brief Основной конструктор
    //! \param TCPPort порт, на котором создается сервер
    public SubserverNetworker(int TCPPort){

        this.TCPPort = TCPPort;
        tcpSenders = new HashSet<TCPSender>();
        try {
            //sender = new UDPSender(UDPPort);
            tcpSocket = new ServerSocket(TCPPort);
            tcpSocket.setReuseAddress(true);

            connecter = new ClientConnecter(tcpSocket);
            //tcpSenders.add(new TCPSender(tcpSocket));
        } catch (Exception ex){
            System.out.println(ex.getMessage());
        }
    }

    //! \brief Основная функция отправки сообщения
    //! \param message Сообщение, которое отправляем
    public void send_message(MessageTCP message){
        try {

            Client receiver = message.sender;
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

        } catch(MafiaException ex){
            System.out.println(ex.getMessage());
        }
    }

    //! \brief Функция обработки приходящих событий (подключение, отключение клиентов и получение сообщений)
    public void on_event(MafiaEvent event){
        //System.out.println("on_event in networker");
        if(event instanceof NetworkEvent){
            if(event instanceof ClientConnected){
                try {
                    System.out.print("Client connected: ");
                    System.out.print(((ClientConnected) event).client.ip);
                    System.out.print(" ");
                    System.out.println(((ClientConnected) event).client.port);
                    tcpSenders.add(new TCPSender(((ClientConnected) event).socket));
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
    //!\param message Сообщение, которое собрал MainServerNetworker
    private void process_message(MessageTCP message){
        if(message.id == MessageType.TestText) {
            //System.out.println("MESSAGE RECEIVED!!! PROCESSING!!!");
            System.out.println(message);
            //System.out.println("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");
            System.out.println("Received message from: " + message.sender.ip.toString() + ":" +
                    String.valueOf(message.sender.port));
            System.out.println("Message type: " + message.id);
            //System.out.println("ID: " + messageJ.Getid());
            System.out.println("Data: " + String.valueOf(message.data.toString()));
            send_message(message);
        }
    }

}
