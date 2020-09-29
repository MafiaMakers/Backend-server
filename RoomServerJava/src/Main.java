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

        SubserverNetworker networker = new SubserverNetworker(7373, 5000);

        ClientNetworker c_networker = new ClientNetworker(5001, 7373);

        /*MessageJ testMessage = new MessageJ();
        testMessage.Setdata("Test".toCharArray());
        testMessage.Setid(179);
        testMessage.SetpartIndex(0);
        testMessage.SetpartsCount(1);
        testMessage.Settype(1);
        testMessage.Getclient().Setip(0x7F000001);
        testMessage.Getclient().Setport(5000);

        networker.send_message(testMessage);*/

        /*MessageJ testMessage1 = new MessageJ();
        testMessage.Setdata("Test1".toCharArray());
        testMessage.Setid(180);
        testMessage.SetpartIndex(0);
        testMessage.SetpartsCount(1);
        testMessage.Settype(2);
        testMessage.Getclient().Setip(0x7F000001);
        testMessage.Getclient().Setport(7373);

        networker.send_message(testMessage);*/

        /*MessageJ testMessage2 = new MessageJ();
        testMessage2.Setdata("Test2".toCharArray());
        testMessage2.Setid(181);
        testMessage2.SetpartIndex(0);
        testMessage2.SetpartsCount(1);
        testMessage2.Settype(1);
        testMessage2.Getclient().Setip(0x7F000001);
        testMessage2.Getclient().Setport(5000);

        c_networker.send_message(testMessage2);*/

        MessageJ testMessage3 = new MessageJ();
        testMessage3.Setdata("Test3".toCharArray());
        testMessage3.Setid(182);
        testMessage3.SetpartIndex(0);
        testMessage3.SetpartsCount(1);
        testMessage3.Settype(2);
        testMessage3.Getclient().Setip(0x7F000001);
        testMessage3.Getclient().Setport(7373);

        c_networker.send_message(testMessage3);

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
