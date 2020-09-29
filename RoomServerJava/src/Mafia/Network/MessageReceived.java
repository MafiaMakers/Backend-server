package Mafia.Network;

public class MessageReceived extends NetworkEvent {
    protected MessageJ message;

    public MessageReceived(MessageJ message, Object source){
        //System.out.println("MessageReceived event constructor");
        this.message = message;
        //System.out.println(message.Getdata());
    }

    public MessageJ get_message(){
        return message;
    }
}
