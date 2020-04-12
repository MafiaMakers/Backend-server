#ifndef DEFINES_H
#define DEFINES_H

#define BUF_SIZE 4096
#define CLIENTS_MAX_COUNT 64
#define CASUAL_PORT 53000
#define IP_PROTO AF_INET

#define SERVER_INITIALIZATION_ERROR 100
#define RECEIVING_ERROR 101
#define SHORT_MESSAGE_ERROR 102
#define ROOM_ID_ERROR 103
#define CONTROL_SUM_ERROR 104
#define UNKNOWN_MES_ID_ERROR 105
#define CLIENTS_LIMIT_ERROR 106
#define REPEATATIVE_REQUEST_ERROR 107
#define CLOSED_ROOM_REQUEST_ERROR 108
#define MESSAGE_SIZE_ERROR 109
#define PRIVACY_ERROR 110

#define RESEND_MESSAGE_ID 1
#define TEXT_MESSAGE_ID 2
#define CONNECT_MESSAGE_ID 3
#define ERROR_MESSAGE_ID 4
#define SUCCESS_MESSAGE_ID 5
#define EXIT_ROOM_MESSAGE_ID 6
#define CHANGE_NAME_MESSAGE_ID 7

#endif // DEFINES_H
