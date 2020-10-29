package Mafia.Network.General;

//! \brief Событие получения сообщения
public class MessageReceived extends NetworkEvent {
    //! \brief Сообщение, которое было получено
    protected MessageTCP message;

    //! \brief Основной конструктор, который задает поля
    public MessageReceived(MessageTCP message, Object source){
        super(source);
        this.message = message;
    }

    //! \brief Получение сообщения, которое получено ))
    public MessageTCP get_message(){
        return message;
    }
}
