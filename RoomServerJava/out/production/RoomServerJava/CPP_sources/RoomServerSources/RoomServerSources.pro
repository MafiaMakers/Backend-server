TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += "C:\Program Files\Java\jdk-9.0.4\include"
INCLUDEPATH += "..\..\..\..\..\..\Server"
INCLUDEPATH += ".\"
SOURCES += \
        IncludeList.cpp \
        Main.cpp \
        Summator.cpp \
        includemessagesender.cpp

HEADERS += \
    IncludeList.h \
    Main.h \
    Summator.h \
    defnetworkersignals.h \
    includemessagesender.h
