package Mafia.Network;
//!\brief Класс, который представляет собой плюсовскую структуру Mafia::Network::Client
public class ClientJ {
	static {
		//Загрузка .so файла с определением нативных функций
		System.load(ClientJ.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/","\\\\") + "Mafia\\Network\\CPP_LIB\\ClientJ.so");
	}

	//Указатель на плюсовский Mafia::Network::Client в памяти
	private long _pointer = 0;

	//!\brief Метод создания объекта
	private native void init();
	//!\brief По сути деструктор
	protected native void finalize() throws Throwable;
	//!\brief Метод проверки равенства
	public native boolean equals(ClientJ second);

	//!\brief Пустой конструктор
	public ClientJ() {
		init();
	}
	//!\brief Системный конструктор!!! Не используйте его
	public ClientJ(long pointer) {
		_pointer = pointer;
	}
	//!\brief Полный конструктор, инициализирующий все поля
	public ClientJ(int _ip, int _port) {
		init();
		Setip(_ip);
		Setport(_port);
	}

	//Геттеры и сеттеры

	public native int Getip();
	public native void Setip(int data);
	public native int Getport();
	public native void Setport(int data);
}
