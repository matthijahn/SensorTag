#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T13:24:37
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SensorTag
TEMPLATE = app


SOURCES += main.cpp\
        sensortag.cpp \
    scanner.cpp

HEADERS  += sensortag.h \
    scanner.h

FORMS    += sensortag.ui \
    scanner.ui
