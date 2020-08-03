QT += testlib
QT -= gui
QT += sql network

INCLUDEPATH += "../Server"

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../Server/Database/chatdatabasemanager.cpp \
    ../Server/Database/chatmessage.cpp \
    ../Server/Database/chatsettingsdatabasemanager.cpp \
    ../Server/Database/databasehelper.cpp \
    ../Server/Database/databasemanager.cpp \
    ../Server/Database/databaseworker.cpp \
    ../Server/Database/gamedatabasemanager.cpp \
    ../Server/Database/transactiondatabasemanager.cpp \
    ../Server/Database/user.cpp \
    ../Server/Database/userdatabasemanager.cpp \
    ../Server/Exceptions/databaseworkingexception.cpp \
    ../Server/Exceptions/exception.cpp \
    ../Server/Exceptions/gameplayexception.cpp \
    ../Server/Exceptions/mainserverexception.cpp \
    ../Server/Exceptions/messageparsingexception.cpp \
    ../Server/Exceptions/messageprocessingexception.cpp \
    ../Server/Exceptions/requestexception.cpp \
    ../Server/Exceptions/subserverexception.cpp \
    ../Server/Exceptions/systemexception.cpp \
    ../Server/Gameplay/game_s.cpp \
    ../Server/Network/MessageProcessing/addusertochatprocessorobject.cpp \
    ../Server/Network/MessageProcessing/changeemailrequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/changenicknameprocessorobject.cpp \
    ../Server/Network/MessageProcessing/changeuserscapabilityprocessorobject.cpp \
    ../Server/Network/MessageProcessing/confirmemailprocessorobject.cpp \
    ../Server/Network/MessageProcessing/createchatrequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/creategamerequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/createuserprocessorobject.cpp \
    ../Server/Network/MessageProcessing/deletechatmessageprocessorobject.cpp \
    ../Server/Network/MessageProcessing/editchatmessageprocessorobject.cpp \
    ../Server/Network/MessageProcessing/getchatmessagesprocessorobject.cpp \
    ../Server/Network/MessageProcessing/getchatsrequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/getintogamerequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/getstatisticsrequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/loginrequestprocessorobject.cpp \
    ../Server/Network/MessageProcessing/logoutprocessorobject.cpp \
    ../Server/Network/MessageProcessing/logsdataprocessorobject.cpp \
    ../Server/Network/MessageProcessing/processorobject.cpp \
    ../Server/Network/MessageProcessing/readchatmessageprocessorobject.cpp \
    ../Server/Network/MessageProcessing/removeuserfromchatprocessorobject.cpp \
    ../Server/Network/MessageProcessing/sendchatmessageprocessorobject.cpp \
    ../Server/Network/crypto.cpp \
    ../Server/Network/mainservernetworker.cpp \
    ../Server/Network/messageprocessor.cpp \
    ../Server/Requests/networkrequest.cpp \
    ../Server/Requests/request.cpp \
    ../Server/Subservers/backupsubserverobject.cpp \
    ../Server/Subservers/roomsubserverobject.cpp \
    ../Server/Subservers/subserverobject.cpp \
    ../Server/System/SHA_256.cpp \
    ../Server/System/functions.cpp \
    ../Server/System/keygen.cpp \
    ../Server/System/logsmanager.cpp \
    ../Server/System/mailsender.cpp \
    ../Server/System/portsmanager.cpp \
    ../Server/System/runprogram.cpp \
    ../Server/System/serializer.cpp \
    ../Server/System/simplestring.cpp \
    ../Server/clientinfo.cpp \
    ../Server/mainservermanager.cpp \
    ../Server/userstatistics.cpp \
    Database/tst_chatdatabasemanager.cpp \
    Database/tst_chatsettingsdatabasemanager.cpp \
    Database/tst_databasehelper.cpp \
    Database/tst_databaseworker.cpp \
    Database/tst_gamedatabasemanager.cpp \
    Database/tst_transactiondatabasemanager.cpp \
    Database/tst_userdatabasemanager.cpp \
    Network/tst_crypto.cpp \
    Network/tst_mainservernetworker.cpp \
    System/tst_SHA_256.cpp \
    System/tst_Tuple.cpp \
    System/tst_functions.cpp \
    System/tst_keygen.cpp \
    System/tst_limitedqueue.cpp \
    System/tst_portsmanager.cpp \
    System/tst_serializer.cpp \
    System/tst_simplestring.cpp \
    main.cpp

HEADERS += \
    ../Server/Database/chatdatabasemanager.h \
    ../Server/Database/chatmessage.h \
    ../Server/Database/chatsettingsdatabasemanager.h \
    ../Server/Database/databasehelper.h \
    ../Server/Database/databasemanager.h \
    ../Server/Database/databaseworker.h \
    ../Server/Database/gamedatabasemanager.h \
    ../Server/Database/transactiondatabasemanager.h \
    ../Server/Database/user.h \
    ../Server/Database/userdatabasemanager.h \
    ../Server/Exceptions/databaseworkingexception.h \
    ../Server/Exceptions/exception.h \
    ../Server/Exceptions/gameplayexception.h \
    ../Server/Exceptions/mainserverexception.h \
    ../Server/Exceptions/messageparsingexception.h \
    ../Server/Exceptions/messageprocessingexception.h \
    ../Server/Exceptions/requestexception.h \
    ../Server/Exceptions/subserverexception.h \
    ../Server/Exceptions/systemexception.h \
    ../Server/Gameplay/game_s.h \
    ../Server/Gameplay/gameitems.h \
    ../Server/Network/MessageProcessing/addusertochatprocessorobject.h \
    ../Server/Network/MessageProcessing/changeemailrequestprocessorobject.h \
    ../Server/Network/MessageProcessing/changenicknameprocessorobject.h \
    ../Server/Network/MessageProcessing/changeuserscapabilityprocessorobject.h \
    ../Server/Network/MessageProcessing/confirmemailprocessorobject.h \
    ../Server/Network/MessageProcessing/createchatrequestprocessorobject.h \
    ../Server/Network/MessageProcessing/creategamerequestprocessorobject.h \
    ../Server/Network/MessageProcessing/createuserprocessorobject.h \
    ../Server/Network/MessageProcessing/deletechatmessageprocessorobject.h \
    ../Server/Network/MessageProcessing/editchatmessageprocessorobject.h \
    ../Server/Network/MessageProcessing/getchatmessagesprocessorobject.h \
    ../Server/Network/MessageProcessing/getchatsrequestprocessorobject.h \
    ../Server/Network/MessageProcessing/getintogamerequestprocessorobject.h \
    ../Server/Network/MessageProcessing/getstatisticsrequestprocessorobject.h \
    ../Server/Network/MessageProcessing/loginrequestprocessorobject.h \
    ../Server/Network/MessageProcessing/logoutprocessorobject.h \
    ../Server/Network/MessageProcessing/logsdataprocessorobject.h \
    ../Server/Network/MessageProcessing/processorobject.h \
    ../Server/Network/MessageProcessing/readchatmessageprocessorobject.h \
    ../Server/Network/MessageProcessing/removeuserfromchatprocessorobject.h \
    ../Server/Network/MessageProcessing/sendchatmessageprocessorobject.h \
    ../Server/Network/crypto.h \
    ../Server/Network/mainservernetworker.h \
    ../Server/Network/message.h \
    ../Server/Network/messageTypes.h \
    ../Server/Network/messageprocessor.h \
    ../Server/Requests/networkrequest.h \
    ../Server/Requests/request.h \
    ../Server/Subservers/backupsubserverobject.h \
    ../Server/Subservers/roomsubserverobject.h \
    ../Server/Subservers/subserverobject.h \
    ../Server/System/SHA_256.h \
    ../Server/System/functions.h \
    ../Server/System/keygen.h \
    ../Server/System/limitedqueue.h \
    ../Server/System/logsmanager.h \
    ../Server/System/mailsender.h \
    ../Server/System/portsmanager.h \
    ../Server/System/runprogram.h \
    ../Server/System/serializer.h \
    ../Server/System/simplestring.h \
    ../Server/System/tuple.h \
    ../Server/mainservermanager.h \
    Database/DatabaseTests \
    Database/tst_chatdatabasemanager.h \
    Database/tst_chatsettingsdatabasemanager.h \
    Database/tst_databasehelper.h \
    Database/tst_databaseworker.h \
    Database/tst_gamedatabasemanager.h \
    Database/tst_transactiondatabasemanager.h \
    Database/tst_userdatabasemanager.h \
    Network/tst_crypto.h \
    Network/tst_mainservernetworker.h \
    System/tst_SHA_256.h \
    System/tst_Tuple.h \
    System/tst_functions.h \
    System/tst_keygen.h \
    System/tst_limitedqueue.h \
    System/tst_portsmanager.h \
    System/tst_serializer.h \
    System/tst_simplestring.h \
    test.h

DISTFILES += \
    Network/NetworkTests \
    System/SystemTests
