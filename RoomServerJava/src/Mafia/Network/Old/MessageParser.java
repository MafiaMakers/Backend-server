package Mafia.Network.Old;

public class MessageParser {

    static{
        System.load(
                MessageParser.class.getProtectionDomain().getCodeSource().getLocation().getPath()
                        .substring(1).replaceAll("/", "\\\\") + "Mafia\\Network\\CPP_LIB\\MessageParser.so");

        init();
    }

    private static native void init();

    public static native MessageJ parse_message(byte[] data, int size, int ip, int port);

    public static native byte[] wrap_message(MessageJ source);
}
