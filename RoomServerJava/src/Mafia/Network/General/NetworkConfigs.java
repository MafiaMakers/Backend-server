package Mafia.Network.General;

import java.net.InetAddress;

//! \brief Класс, отвечающий за конфиги сети. Пока просто константы. Но потом прикручу чтение файлов.
public class NetworkConfigs {
    //! \brief Размер буфера сообщений, которые читаем. Можно и побольше поставить
    private static final int BUFFER_SIZE = 1024;
    //! \brief ip сервера, к которому должен подключаться клиент. Пока стоит локалхост. Но потом заменим на ip сервера
    private static final String SERVER_IP = "127.0.0.1";

    //! \brief Функция получения размера буфера сообщений \ref BUFFER_SIZE вынесено в отдельную функцию, так как потом
    //! \brief переменные будут уже не константами, а читаться из файла, поэтмоу лучше из функции все вызывать
    public static int get_buffer_size(){
        return BUFFER_SIZE;
    }

    //! \brief Функция получения ip сервера
    public static String get_server_ip(){
        return SERVER_IP;
    }

}
