TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -lws2_32
QMAKE_CXXFLAGS += -Wall
LIBS += -lws2_32
SOURCES += \
        main.cpp \
        networker.cpp \
        systemfunctions.cpp

DISTFILES += \
    MessageProto.txt

HEADERS += \
    defines.h \
    include.h \
    networker.h \
    systemfunctions.h
