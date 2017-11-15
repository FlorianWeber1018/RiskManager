#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T10:23:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Risk_Manager
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


SOURCES += \
        main.cpp \
        risk_manager.cpp \
    qtreewidget_toxml.cpp \
    editseletableriskreduction.cpp \
    qtreewidget_totextable.cpp \
    editdevicespecifications.cpp

HEADERS += \
        risk_manager.h \
    qtreewidget_toxml.h \
    editseletableriskreduction.h \
    qtreewidget_totextable.h \
    editdevicespecifications.h

FORMS += \
        risk_manager.ui \
    editseletableriskreduction.ui \
    editdevicespecifications.ui
