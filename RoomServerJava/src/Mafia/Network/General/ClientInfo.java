package Mafia.Network.General;

import Mafia.Gameplay.AccountType;

//! \brief Структура информации о пользователе. Подробнее можно смотреть на основном сервере
public class ClientInfo {
    private Client client;
    private int userId;
    private String name;
    private AccountType accountType;
    private int[] victoriesByRoles;
    private int[] defeatesByRoles;
}
