package Mafia.Network.General;

import com.fasterxml.jackson.databind.JsonNode;

//! \brief Основная структура сообщения
public class MessageTCP {
    public MessageType id;
    public Client sender;
    public JsonNode data;
}
