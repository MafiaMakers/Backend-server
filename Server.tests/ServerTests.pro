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
    ../Server/System/SHA_256.cpp \
    ../Server/System/functions.cpp \
    ../Server/System/keygen.cpp \
    ../Server/System/mailsender.cpp \
    ../Server/System/portsmanager.cpp \
    ../Server/System/runprogram.cpp \
    ../Server/System/serializer.cpp \
    ../Server/userstatistics.cpp \
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
    System/SystemTests
