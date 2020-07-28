
QT -= gui
QT += network
QT += sql
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_CXXFLAGS_WARN_ON += -Wall

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Database/chatdatabasemanager.cpp \
        Database/chatmessage.cpp \
        Database/chatsettingsdatabasemanager.cpp \
        Database/databasehelper.cpp \
        Database/databasemanager.cpp \
        Database/databaseworker.cpp \
        Database/gamedatabasemanager.cpp \
        Database/transactiondatabasemanager.cpp \
        Database/user.cpp \
        Database/userdatabasemanager.cpp \
        Exceptions/databaseworkingexception.cpp \
        Exceptions/exception.cpp \
        Exceptions/gameplayexception.cpp \
        Exceptions/mainserverexception.cpp \
        Exceptions/messageparsingexception.cpp \
        Exceptions/messageprocessingexception.cpp \
        Exceptions/requestexception.cpp \
        Exceptions/subserverexception.cpp \
        Exceptions/systemexception.cpp \
        Gameplay/game_s.cpp \
        Network/MessageProcessing/addusertochatprocessorobject.cpp \
        Network/MessageProcessing/changeemailrequestprocessorobject.cpp \
        Network/MessageProcessing/changenicknameprocessorobject.cpp \
        Network/MessageProcessing/changeuserscapabilityprocessorobject.cpp \
        Network/MessageProcessing/confirmemailprocessorobject.cpp \
        Network/MessageProcessing/createchatrequestprocessorobject.cpp \
        Network/MessageProcessing/creategamerequestprocessorobject.cpp \
        Network/MessageProcessing/createuserprocessorobject.cpp \
        Network/MessageProcessing/deletechatmessageprocessorobject.cpp \
        Network/MessageProcessing/editchatmessageprocessorobject.cpp \
        Network/MessageProcessing/getchatmessagesprocessorobject.cpp \
        Network/MessageProcessing/getchatsrequestprocessorobject.cpp \
        Network/MessageProcessing/getintogamerequestprocessorobject.cpp \
        Network/MessageProcessing/getstatisticsrequestprocessorobject.cpp \
        Network/MessageProcessing/loginrequestprocessorobject.cpp \
        Network/MessageProcessing/logoutprocessorobject.cpp \
        Network/MessageProcessing/logsdataprocessorobject.cpp \
        Network/MessageProcessing/processorobject.cpp \
        Network/MessageProcessing/readchatmessageprocessorobject.cpp \
        Network/MessageProcessing/removeuserfromchatprocessorobject.cpp \
        Network/MessageProcessing/sendchatmessageprocessorobject.cpp \
        Network/crypto.cpp \
        Network/messageprocessor.cpp \
        Subservers/backupsubserverobject.cpp \
        Subservers/roomsubserverobject.cpp \
        System/SHA_256.cpp \
        System/functions.cpp \
        System/keygen.cpp \
        System/logsmanager.cpp \
        System/mailsender.cpp \
        System/portsmanager.cpp \
        System/runprogram.cpp \
        System/serializer.cpp \
        clientinfo.cpp \
        main.cpp \
        mainservermanager.cpp \
        Network/mainservernetworker.cpp \
        Requests/networkrequest.cpp \
        Requests/request.cpp \
        Subservers/subserverobject.cpp \
        userstatistics.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Database/chatdatabasemanager.h \
    Database/chatmessage.h \
    Database/chatsettingsdatabasemanager.h \
    Database/databasehelper.h \
    Database/databasemanager.h \
    Database/databaseworker.h \
    Database/gamedatabasemanager.h \
    Database/transactiondatabasemanager.h \
    Database/user.h \
    Database/userdatabasemanager.h \
    Exceptions/databaseworkingexception.h \
    Exceptions/exception.h \
    Exceptions/gameplayexception.h \
    Exceptions/mainserverexception.h \
    Exceptions/messageparsingexception.h \
    Exceptions/messageprocessingexception.h \
    Exceptions/requestexception.h \
    Exceptions/subserverexception.h \
    Exceptions/systemexception.h \
    Gameplay/game_s.h \
    Gameplay/gameitems.h \
    Network/MessageProcessing/addusertochatprocessorobject.h \
    Network/MessageProcessing/changeemailrequestprocessorobject.h \
    Network/MessageProcessing/changenicknameprocessorobject.h \
    Network/MessageProcessing/changeuserscapabilityprocessorobject.h \
    Network/MessageProcessing/confirmemailprocessorobject.h \
    Network/MessageProcessing/createchatrequestprocessorobject.h \
    Network/MessageProcessing/creategamerequestprocessorobject.h \
    Network/MessageProcessing/createuserprocessorobject.h \
    Network/MessageProcessing/deletechatmessageprocessorobject.h \
    Network/MessageProcessing/editchatmessageprocessorobject.h \
    Network/MessageProcessing/getchatmessagesprocessorobject.h \
    Network/MessageProcessing/getchatsrequestprocessorobject.h \
    Network/MessageProcessing/getintogamerequestprocessorobject.h \
    Network/MessageProcessing/getstatisticsrequestprocessorobject.h \
    Network/MessageProcessing/loginrequestprocessorobject.h \
    Network/MessageProcessing/logoutprocessorobject.h \
    Network/MessageProcessing/logsdataprocessorobject.h \
    Network/MessageProcessing/processorobject.h \
    Network/MessageProcessing/readchatmessageprocessorobject.h \
    Network/MessageProcessing/removeuserfromchatprocessorobject.h \
    Network/MessageProcessing/sendchatmessageprocessorobject.h \
    Network/crypto.h \
    Network/messageprocessor.h \
    Subservers/backupsubserverobject.h \
    Subservers/roomsubserverobject.h \
    System/SHA_256.h \
    System/functions.h \
    System/keygen.h \
    System/limitedqueue.h \
    System/logsmanager.h \
    System/mailsender.h \
    System/portsmanager.h \
    System/runprogram.h \
    System/serializer.h \
    System/tuple.h \
    clientinfo.h \
    mainservermanager.h \
    Network/mainservernetworker.h \
    Network/message.h \
    Network/messagetypes.h \
    Requests/networkrequest.h \
    Requests/request.h \
    System/simplestring.h \
    Subservers/subserverobject.h \
    userstatistics.h

DISTFILES += \
    Mafia \
    MafiaDatabase \
    MafiaExceptions \
    MafiaGameplay \
    MafiaNetwork \
    MafiaRequests \
    MafiaSubservers \
    MafiaSystem
