package Mafia.Network;

import Mafia.General.Network.NetworkEvent;
import Mafia.General.Network.Client;

//! \brief Событие, связанное с действием клиента. От него наследуются \ref ClientLeave и \ref ClientConnected
public class ClientAction extends NetworkEvent {
    //! \brief Клиент, который подключился
    protected Client client;

    ClientAction(Client client){
        this.client = client;
    }

    ClientAction(Client client, Object sender){
        super(sender);
        this.client = client;
    }

    public Client get_client(){
        return client;
    }
}
