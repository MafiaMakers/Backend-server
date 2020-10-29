package Mafia.Network.General;

import Mafia.Events.MafiaEvent;
import Mafia.Events.MafiaEventListener;

//! \brief Основной класс, отвечающий за работу с сетью на клиенте.
public class ClientNetworker extends MafiaEventListener {
    //! \brief Объект для отправки сообщений
    private TCPSender tcpSender;

    //! \brief Основной конструктор
    //! \param tcpPort порт сервера, к которому надо подключиться
    public ClientNetworker(int tcpPort){
        try {
            tcpSender = new TCPSender(tcpPort);
        } catch (Exception ex){
            System.out.println(ex.getMessage());
        }
    }

    //! \brief Функция отправки сообщеня
    public void send_message(MessageTCP message){
        tcpSender.send_message(message);
    }

    //! \brief Наследованная функция обработки входящего события. Тут пока заглушка. Надо будеть поставить генерацию нового события или перенаправление на выполнение какой-нибудь другой функции
    public void on_event(MafiaEvent event){
        if(event instanceof MessageReceived){
            System.out.println("Received message on client:");
            System.out.println(((MessageReceived) event).get_message());
        }
    }
}
