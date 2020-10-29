package Mafia.Network.General;

import com.fasterxml.jackson.core.*;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.*;
import com.fasterxml.jackson.databind.node.ObjectNode;


import java.io.IOException;
import java.util.List;

//! \bried Класс парсинга сообщений в формате json.
public class MessageParser {
    //! \brief Библиотечный объект для парсинга
    private static ObjectMapper mapper;

    static {
        //! \brief Статическая инициализация этого объекта
        mapper = new ObjectMapper();
    }

    //! \brief Функция парсинга. Возвращает объект, по сути словарь... но не совсем
    //! \param data Строка с данными, которую парсим
    public static JsonNode parse(String data) throws IOException {
        return mapper.readTree(data);
    }

    //! \brief Функция заворачивания сообщения в строку json
    //! \param message Сообщение, которое оборачиваем
    public static String wrap_message(MessageTCP message){
        ObjectNode result = mapper.createObjectNode();
        result.put("id", message.id.getType());
        result.put("data", message.data);
        return result.toString();
    }
}
