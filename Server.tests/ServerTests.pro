QT += testlib
QT -= gui
QT += sql network

INCLUDEPATH += "../Server"

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


SOURCES +=  \
    ../Server/Exceptions/exception.cpp \
    ../Server/Exceptions/systemexception.cpp \
    ../Server/System/serializer.cpp \
    ../Server/userstatistics.cpp \
    System/tst_Tuple.cpp \
    System/tst_serializer.cpp \
    main.cpp

HEADERS += \
    System/tst_Tuple.h \
    System/tst_serializer.h
