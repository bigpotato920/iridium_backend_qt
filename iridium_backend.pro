#-------------------------------------------------
#
# Project created by QtCreator 2013-03-26T14:16:00
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iridium_backend
TEMPLATE = app


SOURCES += main.cpp\
        iridiumdialog.cpp \
    coordinate.cpp

HEADERS  += iridiumdialog.h \
    coordinate.h

FORMS    += iridiumdialog.ui
