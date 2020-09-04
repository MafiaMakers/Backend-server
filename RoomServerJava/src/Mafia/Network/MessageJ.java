package Mafia.Network;
//!\brief Класс, который представляет плюсовскую структуру Mafia::Network::Message
public class MessageJ {
	static {
		//Загрузка .so файла с определением нативных функций
		System.load(MessageJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") + "Mafia\\Network\\CPP_LIB\\MessageJ.so");
	}

	//Указатель на плюсовский Mafia::Network::Client в памяти
	private long _pointer = 0;
	//!\brief Метод создания объекта
	private native void init();
	//!\brief По сути деструктор
	protected native void finalize() throws Throwable;
	//!\brief Метод проверки равенства
	public native boolean equals(MessageJ second);

	//!\brief Пустой конструктор
	public MessageJ() {
		init();
	}
	//!\brief Системный конструктор!!! Не используйте его
	public MessageJ(long pointer) {
		_pointer = pointer;
	}
	//!\brief Полный конструктор, инициализирующий все поля
	public MessageJ(int _id, int _partsCount, int _partIndex, int _type, Mafia.Network.ClientJ _client, char[] _data) {
		init();
		Setid(_id);
		SetpartsCount(_partsCount);
		SetpartIndex(_partIndex);
		Settype(_type);
		Setclient(_client);
		Setdata(_data);
	}

	//Геттеры и сеттеры

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
