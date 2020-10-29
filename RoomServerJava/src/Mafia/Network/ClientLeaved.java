package Mafia.Network;

import Mafia.Network.General.Client;

//! \brief Событие, вызываемое при покидании сервера клиентом
public class ClientLeaved extends ClientAction {
    public ClientLeaved(Client client){
        super(client);
    }
}
