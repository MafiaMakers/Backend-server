package Mafia.Network;

public class SubserverNetworker {
    static {
        System.load(MessageJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") +
                "Mafia\\Network\\CPP_LIB\\SubserverNetworker.so");
    }

    private static long networkerPointer = 0;

    public static native void init(int port);

    public native static int sendMessage(MessageJ messageJ);

    public static native void processReceivedBytes(byte[] data, int size, int ip, int port);

    public static void process_message(MessageJ messageJ){
        //System.out.println("\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n");
        //System.out.println("Received message from: " + String.valueOf(messageJ.Getclient().Getip()) + ":" + String.valueOf(messageJ.Getclient().Getport()));
        //System.out.println("Message type: " + String.valueOf(messageJ.Gettype()));
        System.out.println("ID: " + String.valueOf(messageJ.Getid()));
        //System.out.println("Data: " + String.valueOf(String.valueOf(messageJ.Getdata())));
        System.out.println("OK\nOK\nOK");
    }
}
