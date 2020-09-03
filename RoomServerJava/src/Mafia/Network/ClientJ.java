package Mafia.Network;
public class ClientJ {
	static {
		System.load(ClientJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") + "Mafia\\Network\\CPP_LIB\\ClientJ.so");
	}

	private long _pointer = 0;

	private native void init();
	protected native void finalize() throws Throwable;
	public native boolean equals(ClientJ second);


	public ClientJ() {
		init();
	}

	public ClientJ(long pointer) {
		_pointer = pointer;
	}

	public ClientJ(int _ip, int _port) {
		init();
		Setip(_ip);
		Setport(_port);
	}


	public native int Getip();
	public native void Setip(int data);
	public native int Getport();
	public native void Setport(int data);
}
