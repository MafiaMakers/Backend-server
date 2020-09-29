package Mafia.Network;

public class ClientConnected extends ClientAction {

    ClientConnected(ClientJ client){
        super(client);
    }

    ClientConnected(ClientJ client, Object sender){
        super(client, sender);
    }

}
