#-------------------------------------------------
#
# Project created by QtCreator 2016-09-27T09:57:05
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SensorTagv2
TEMPLATE = app


SOURCES += main.cpp\
        sensortag.cpp \
    scanner.cpp

HEADERS  += sensortag.h \
    scanner.h

FORMS    += sensortag.ui \
    scanner.ui
