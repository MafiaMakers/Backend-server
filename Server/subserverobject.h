#ifndef SUBSERVEROBJECT_H
#define SUBSERVEROBJECT_H

#include <QObject>
#include "Network/message.h"
#include "Network/messageTypes.h"
#include "System/simplestring.h"
#include "Exceptions/exception.h"
#include "Requests/networkrequest.h"

typedef int RequestIdType;

namespace Mafia {

    struct RequestWithId{
        NetworkRequest* request;
        RequestIdType id;
    };

    class SubserverObject : public QObject
    {
        Q_OBJECT
    public:

        explicit SubserverObject(QObject *parent = nullptr);

        SubserverObject(MainServerNetworker* networker, int port, const String path, const String processName,
                        int checkInterval = 2000, int maxNotAnswering = 5, const String specialCommands = String(""));

        virtual int send_request(MessageTypeType type, SymbolType* data, int size);

        virtual void pass_client(Client client);

        template<class T>
        T get_request_result(int requestId);
    signals:
        void on_request_ready(int requestId);

        void on_crash();

        void on_message_got(Message message);
    protected slots:
        virtual void request_answered(Request* request);

        virtual void message_from_server(Message message);

        virtual void crash_processing();
    protected:

        virtual void _check_subserver_respond();

        virtual void run_me(const String specialCommands);

        Client myAddress;

        QList<RequestWithId> currentRequests;

        MainServerNetworker* networker;

        int pid;

        bool initialized = false;

        int notAnsweringsCount = 0;

        const String path;

        const String processName;

        const int checkRespondInterval = 2000;

        const int maxNotAnsweringsCount = 5;
    };
}


#endif // SUBSERVEROBJECT_H
