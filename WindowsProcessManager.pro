#-------------------------------------------------
#
# Project created by QtCreator 2025-05-13T20:14:26
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProcessManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ProcessUtils.cpp \    
    processfetcher.cpp

HEADERS += \
    mainwindow.h \
    ProcessUtils.h \
    processutils.h \
    processfetcher.h

FORMS += \
    mainwindow.ui

# Windows-specific
LIBS += -lPsapi -lUser32

DISTFILES += \
    style.qss
QT += winextras
LIBS += -liphlpapi -lws2_32 -lpsapi
