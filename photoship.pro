#-------------------------------------------------
#
# Project created by QtCreator 2017-12-26T18:00:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photoship
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        photoship.cpp \
    rgbdialog.cpp \
    huedialog.cpp \
    saturdialog.cpp \
    brightdialog.cpp \
    graphdialog.cpp

HEADERS  += photoship.h \
    rgbdialog.h \
    huedialog.h \
    saturdialog.h \
    brightdialog.h \
    graphdialog.h

FORMS    += photoship.ui \
    rgbdialog.ui \
    huedialog.ui \
    saturdialog.ui \
    brightdialog.ui \
    graphdialog.ui
