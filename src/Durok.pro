#-------------------------------------------------
#
# Project created by QtCreator 2010-11-23T08:04:26
#
#-------------------------------------------------

QT       += core gui

TARGET = Durok
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formhelp.cpp \
    processthread.cpp \
    forminput.cpp

HEADERS  += mainwindow.h \
    formhelp.h \
    processthread.h \
    forminput.h

FORMS    += mainwindow.ui \
    formhelp.ui \
    forminput.ui

OTHER_FILES += \
    help.html

RESOURCES += \
    Durok.qrc
