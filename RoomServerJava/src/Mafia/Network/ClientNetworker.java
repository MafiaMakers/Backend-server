package Mafia.Network;

import Mafia.Events.MafiaEvent;
import Mafia.Events.MafiaEventListener;
import Mafia.Exceptions.MafiaException;

public class ClientNetworker extends MafiaEventListener {
    private UDPSender udpSender;
    private TCPClient tcpSender;

    public ClientNetworker(int udpPort, int tcpPort){
        try {
            udpSender = new UDPSender(udpPort);
            tcpSender = new TCPClient(tcpPort, 777);
        } catch (Exception ex){
            System.out.println(ex.getMessage());
        }
    }

    public void send_message(MessageJ message){
        try {
            if (NetworkConfigs.udpOrTcpMessage(MessageType.from_int(message.Gettype()))) {
                udpSender.send_message(message);
            } else{
                tcpSender.send_message(message);
            }
        } catch(MafiaException ex){
            System.out.println(ex.getMessage());
        }
    }

    public void on_event(MafiaEvent event){
        if(event instanceof MessageReceived){
            System.out.println("Received message on client:");
            System.out.println(((MessageReceived) event).get_message().Getdata());
        }
    }
}
