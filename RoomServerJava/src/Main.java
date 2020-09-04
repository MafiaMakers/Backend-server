import Mafia.Network.*;

import java.net.InetAddress;
import java.nio.ByteBuffer;
import Mafia.*;

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
        /*try {
            SubserverNetworker.init(5000);
            System.out.println("INIT successfully finished");
            MessageJ testMessage = new MessageJ();
            testMessage.Setdata("Test".toCharArray());
            testMessage.Setid(179);
            testMessage.SetpartIndex(0);
            testMessage.SetpartsCount(1);
            testMessage.Settype(17);
            testMessage.Getclient().Setip(0x7F000001);
            testMessage.Getclient().Setport(5000);

            SubserverNetworker.sendMessage(testMessage);
        } catch (IllegalMonitorStateException exception){
            System.out.println(exception.getMessage());
        }*/
    }

}
