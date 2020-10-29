package Mafia.Network.General;

import org.junit.Assert;

import java.net.InetAddress;

//! \brief По сути структура сетевого адреса
public class Client {
    //! \brief IP клиента - 4 байта. Например, 127, 0, 0, 1
    public byte[] ip;
    //! \brief Порт клиента. Просто число 0-65535
    public int port;

    //! \brief Пустой конструктор, который задает порт равный -1 (не инициализирован)
    public Client(){
        ip = new byte[4];
        port = -1;
    }

    //! \brief Основной конструктор, который задает поля
    public Client(byte[] ip, int port){
        assert ip.length == 4;
        this.ip = ip;
        this.port = port;
    }
}
