package Mafia.Network;

import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;

import java.awt.*;
import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

public class NetworkConfigs {
    private static Set<MessageType> udpMessages = new HashSet<MessageType>();
    private static Set<MessageType> tcpMessages = new HashSet<MessageType>();

    private static int BUFFER_SIZE = 1024;

    static {
        udpMessages.add(MessageType.TestUDPMessage);
        tcpMessages.add(MessageType.TestTCPMessage);
    }

    public static int get_buffer_size(){
        return BUFFER_SIZE;
    }

    public static boolean udpOrTcpMessage(MessageType type) throws MafiaException {
        if(tcpMessages.contains(type)){
            return false;
        }
        if(udpMessages.contains(type)){
            return true;
        }
        throw MafiaException.generate(ExceptionId.MessageTypeException, "Unknown message type");
    }
}
