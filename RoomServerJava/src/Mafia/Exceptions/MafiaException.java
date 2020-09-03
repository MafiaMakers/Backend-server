package Mafia.Exceptions;

public class MafiaException extends Exception {
    private int id;

    public int get_id(){
        return id;
    }

    private MafiaException(int id, String data) {
        super(data);
        this.id = id;
    }

    public static MafiaException generate(int id, String data){
        return new MafiaException(id, data);
    }

    public void show(){
        System.out.println("Exception!\nID: " + String.valueOf(id) + "\ndata: " + getMessage());
    }
}
