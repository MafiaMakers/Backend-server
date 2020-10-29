package Mafia.Network.Old;

import Mafia.Network.General.Client;
import Mafia.Network.General.MessageType;

public class MessageUDP {
    public MessageType id;
    public Client sender;
    public int partsCount;
    public int partIndex;
    public byte[] data;
}
