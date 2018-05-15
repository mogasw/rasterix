#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T09:59:52
#
#-------------------------------------------------
include(../common.pri)

QT       += core gui widgets

TARGET = rasterixplugin
TEMPLATE = lib
CONFIG += static

DEFINES += RASTERIXPLUGIN_LIBRARY

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
    worker.cpp \
    gdaldriverhelper.cpp \
    tool.cpp \
    utils/doublespinboxdelegate.cpp \
    utils/colorpickerdelegate.cpp

HEADERS += \
        tool.h \
        rasterixplugin_global.h \ 
    worker.h \
    gdaldriverhelper.h \
    utils/doublespinboxdelegate.h \
    utils/colorpickerdelegate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
