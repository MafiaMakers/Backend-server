package Mafia.Network.Old;

import Mafia.Network.General.Client;

import java.nio.ByteBuffer;

public class UDPMessageParser {
    public static MessageUDP parse(byte[] source, int ip, int port){
        MessageUDP res = new MessageUDP();
        Client client = new Client(ByteBuffer.allocate(4).putInt(ip).array(), port);
        res.sender = client;

        int shift = 0;

        byte[] partsCount = new byte[4];
        for (int i = 0; i < 4; i++){
            partsCount[i] = source[shift];
            shift++;
        }

        byte[] partIndex = new byte[4];
        for (int i = 0; i < 4; i++){
            partsCount[i] = source[shift];
            shift++;
        }

        if(source.length < 8){
            //throw new ...;
        }
        int dataSize = source.length - shift;

        byte[] data = new byte[dataSize];
        for (int i = 0; i < dataSize; i++){
            data[i] = source[shift];
            shift++;
        }
        res.partIndex = ByteBuffer.wrap(partIndex).getInt();
        res.partsCount = ByteBuffer.wrap(partsCount).getInt();
        res.data = data;
        return res;
    }

    public static byte[] wrap(MessageUDP message){

        return new String(message.data).getBytes();
    }
}
