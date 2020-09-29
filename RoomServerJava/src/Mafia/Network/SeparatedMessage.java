package Mafia.Network;

import Mafia.Exceptions.ExceptionId;
import Mafia.Exceptions.MafiaException;
import Mafia.Exceptions.NetworkException;

import java.util.ArrayList;

public class SeparatedMessage {
    private ArrayList<MessageJ> parts;

    MessageJ base;

    public boolean is_complete(){
        for (int i = 0; i < parts.size(); i++){
            if(parts.get(i) == null){
                return false;
            }
        }
        return true;
    }

    public SeparatedMessage(MessageJ firstPart){
        base = new MessageJ(firstPart.Getid(), firstPart.GetpartsCount(),
                -1, firstPart.Gettype(), firstPart.Getclient(), "".toCharArray());
        parts = new ArrayList<MessageJ>();

        for (int i = 0; i < base.GetpartsCount(); i++){
            parts.add(null);
        }

        add(firstPart);
    }

    public boolean matches(MessageJ message){
        if(!base.Getclient().equals(message.Getclient())){
            return false;
        }
        if(base.Gettype() != message.Gettype()){
            return false;
        }
        if(base.Getid() != message.Getid()){
            return false;
        }
        if(base.GetpartsCount() != message.GetpartsCount()){
            return false;
        }
        return true;
    }

    public void add(MessageJ message){
        if(matches(message)){
            parts.set(message.GetpartIndex(), message);
        }
    }

    public MessageJ construct_whole() throws MafiaException {
        if(!is_complete()){
            throw MafiaException.generate(ExceptionId.MessageParsingException,
                    "Attempt to construct not whole message from not completed");
        }

        StringBuilder stringBuilder = new StringBuilder();

        for (int i = 0; i < parts.size(); i++){
            stringBuilder.append(parts.get(i).Getdata());
        }

        base.Setdata(stringBuilder.toString().toCharArray());
        base.SetpartsCount(1);
        base.SetpartIndex(0);
        return base;
    }
}
