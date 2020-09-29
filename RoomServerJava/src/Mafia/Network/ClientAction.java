package Mafia.Network;

public class ClientAction extends NetworkEvent{
    protected ClientJ client;

    ClientAction(ClientJ client){
        this.client = client;
    }

    ClientAction(ClientJ client, Object sender){
        super(sender, "", -1);
        this.client = client;
    }

    public ClientJ get_client(){
        return client;
    }
}
