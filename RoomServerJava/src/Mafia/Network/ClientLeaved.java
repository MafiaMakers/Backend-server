package Mafia.Network;

import Mafia.General.Network.Client;

//! \brief Событие, вызываемое при покидании сервера клиентом
public class ClientLeaved extends ClientAction {
    public ClientLeaved(Client client){
        super(client);
    }
}
