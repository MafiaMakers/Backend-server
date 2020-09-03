package Mafia.Network;
public class MessageJ {
	static {
		System.load(MessageJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") + "Mafia\\Network\\CPP_LIB\\MessageJ.so");
	}

	private long _pointer = 0;

	private native void init();
	protected native void finalize() throws Throwable;
	public native boolean equals(MessageJ second);


	public MessageJ() {
		init();
	}

	public MessageJ(long pointer) {
		_pointer = pointer;
	}

	public MessageJ(int _id, int _partsCount, int _partIndex, int _type, Mafia.Network.ClientJ _client, char[] _data) {
		init();
		Setid(_id);
		SetpartsCount(_partsCount);
		SetpartIndex(_partIndex);
		Settype(_type);
		Setclient(_client);
		Setdata(_data);
	}


	public native int Getid();
	public native void Setid(int data);
	public native int GetpartsCount();
	public native void SetpartsCount(int data);
	public native int GetpartIndex();
	public native void SetpartIndex(int data);
	public native int Gettype();
	public native void Settype(int data);
	public native Mafia.Network.ClientJ Getclient();
	public native void Setclient(Mafia.Network.ClientJ data);
	public native char[] Getdata();
	public native void Setdata(char[] data);
	public native void SetdataElement(char value, int index) throws ArrayIndexOutOfBoundsException;
}
