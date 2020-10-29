package Mafia.Network;

import Mafia.Network.General.Client;

import java.net.Socket;

//! \brief Событие подключения нового клиента
public class ClientConnected extends ClientAction {
    //! \brief Сокет, по которому с этим клиентом можно общаться
    public Socket socket;

    ClientConnected(Client client, Socket socket, Object sender){
        super(client, sender);
        this.socket = socket;
    }

}
