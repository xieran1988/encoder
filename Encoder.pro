#-------------------------------------------------
#
# Project created by QtCreator 2012-01-13T17:38:16
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += network
TARGET = Encoder
TEMPLATE = app

#for QtGstreamer
CONFIG += link_pkgconfig
PKGCONFIG += QtGStreamer-0.10
LIBS += -lQtGStreamer-0.10
LIBS += -lQtGStreamerUtils-0.10
LIBS += -lQtGStreamerUi-0.10

#for libvlc
LIBS += -lvlc

INCLUDEPATH += /usr/include/QtGStreamer

SOURCES += main.cpp\
        mainwindow.cpp \
    mydata.cpp \
    gstmux.cpp \
    group.cpp \
    dlgmux.cpp \
    dlggroup.cpp \
    dlgmix.cpp \
    gstmix.cpp \
    mixview.cpp \
    mixbox.cpp \
    mysocket.cpp

HEADERS  += mainwindow.h \
    mydata.h \
    gstmux.h \
    group.h \
    dlgmux.h \
    dlggroup.h \
    dlgmix.h \
    gstmix.h \
    mixview.h \
    mixbox.h \
    mysocket.h

FORMS    += mainwindow.ui \
    dlgmux.ui \
    dlggroup.ui \
    dlgmix.ui

OTHER_FILES += \
    Group.xml
