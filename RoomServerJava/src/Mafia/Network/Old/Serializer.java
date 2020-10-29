package Mafia.Network.Old;

public class Serializer {
    static {
        System.load(Serializer.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).
                replaceAll("/","\\\\") + "Mafia\\Network\\CPP_LIB\\Serializer.so");
    }

    public static native byte[] serialize(byte[] data);
    public static native byte[] deserialize(byte[] data);

    public static native byte[] serialize(int data);
    public static native int deserialize_int(byte[] data);
}
