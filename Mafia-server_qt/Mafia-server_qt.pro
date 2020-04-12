TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -lws2_32
QMAKE_CXXFLAGS += -Wall
LIBS += -lws2_32

SOURCES += \
        Client.cpp \
        civillian.cpp \
        main_s.cpp \
        networker_s.cpp \
        nonerole.cpp \
        systemfunctions_s.cpp

HEADERS += \
    Client.h \
    IRole.h \
    RolesInclude.h \
    civillian.h \
    defines_s.h \
    include_s.h \
    networker_s.h \
    nonerole.h \
    systemfunctions_s.h

DISTFILES += \
    MessageProto
