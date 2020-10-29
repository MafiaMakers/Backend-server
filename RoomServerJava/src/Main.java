import Mafia.Network.*;

/*! \mainpage Это документация к проекту субсервера-комнаты для онлайн игры в мафию.
 * Приблизительная архитектура: Все хранится в пакете \ref Mafia.
 * В пакете \ref Mafia.Events хранится система событий и основные классы. Все дальнейшие классы событий хранятся в тех пакетах,
 * к которым они относятся по смыслу. В пакете \ref Mafia.Exceptions хранятся все исключения.
 * В пакете \ref Mafia.Gameplay хранится все, что касается правил игры, игрового процесса и прочего (пока этот пакет почти пустой)
 * В пакете \ref Mafia.Gameplay.Roles хранятся все необходимые классы для описания ролей и их поведения.
 * Там же описана важная концепция рутин, на которой во многом будет строиться логика игры. (см. \ref Mafia.Gameplay.Roles.IRoutine)
 * В пакете \ref Mafia.Network находятся классы для работы с сетью. Причем часть классов может быть использована и в
 * кдиентской работе.
 * Пакет \ref Mafia.Network.General содержит как раз классы, необходимые и серверу и клиенту. Остальная часть пакета
 * \ref Mafia.Network содержит классы, которые нужны лишь серверу.\
 * В папке Old находятся старые файлы (UDP и прочее), которые я решил пока не удалять, потому что "а вдруг пригодится"...
 * Тем не менее, в папку Old заглядывать не стоит ))
 * Папка test будет содержать тесты для всех этих классов, но пока их очень мало.
 * Для работы клиента необходимы модули: \ref Mafia.Exceptions \ref Mafia.Events \ref Mafia.Network.General
 */

/*!\brief Класс, из которого выполняется запуск программы. По сути только для этого и нужен
 */
public class Main {

    /*!\brief Основной метод
     *!\param args Аргументы, переданные при вызове (там будет порт и, возможно, что-то еще)
     */
    public static void main(String[] args) {
        try {
            SubserverNetworker networker = new SubserverNetworker(10000);

            /*TCPSender client = new TCPSender(10000);

            System.out.println("Connected");

            MessageTCP message = new MessageTCP();
            message.id = MessageType.from_int(2);
            ObjectMapper mapper = new ObjectMapper();
            String data = "{\"SomeData\": \"Test\"}";
            message.data = mapper.readTree(data);
            client.send_message(message);*/
            //networker.send_message(message);

        } catch (Exception ex){
            ex.printStackTrace();
        }

    }

}
