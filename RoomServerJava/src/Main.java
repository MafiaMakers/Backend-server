import Mafia.Network.*;

/*!\brief Класс, из которого выполняется запуск программы. По сути только для этого и нужен
 */
public class Main {
    /*static {
        System.load(Main.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/", "\\\\") + "CPP_LIB\\Main.so");
    }*/

    /*!\brief Основной метод
     *!\param args Аргументы, переданные при вызове (там будет порт и, возможно, что-то еще)
     */
    public static void main(String[] args) {
        //SubserverNetworker networker = new SubserverNetworker(7373, 5000);

        //ClientNetworker c_networker = new ClientNetworker(5001, 7373);
        byte[] data = "This is test!!!".getBytes();

        //byte[] data1 = "AnotherTest!!!".getBytes();
        int data1 = 179;

        byte[] serialized = Serializer.serialize(data);

        byte[] serialized1 = Serializer.serialize(data1);

        byte[] sum = new byte[serialized.length + serialized1.length];

        for (int i = 0; i < serialized.length; i++){
            //System.out.print((char) serialized[i]);
            sum[i] = serialized[i];
        }


        for (int i = 0; i < serialized1.length; i++){
            sum[i + serialized.length] = serialized1[i];
            //System.out.print((char) serialized1[i]);
        }

        //System.out.println();

        for(int i = 0; i < sum.length; i++){
            //System.out.print((char)sum[i]);
        }

        //System.out.println();
        byte[] result = Serializer.deserialize(sum);

        for (int i = 0; i < result.length; i++){
            System.out.print((char) result[i]);
        }
        System.out.println();
        for (int i = 0; i < sum.length; i++){
            if(sum[i] == 0){
                break;
            }
            //System.out.print((char) sum[i]);
        }

        int result1 = Serializer.deserialize_int(sum);

        System.out.println(result1);
        //System.out.println();

        for (int i = 0; i < sum.length; i++){
            if(sum[i] == 0){
                break;
            }
            //System.out.print((char) sum[i]);
        }
    }

}
