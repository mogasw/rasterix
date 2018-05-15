#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T10:36:55
#
#-------------------------------------------------
include(../common.pri)

QT  += widgets

TARGET = $$qtLibraryTarget(rasterix_export)
TEMPLATE = lib
CONFIG += plugin static

DESTDIR = ../../plugins

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
        exporttool.cpp \
    exportworker.cpp

HEADERS += \
        exporttool.h \
    exportworker.h
DISTFILES += exportplugin.json 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../rasterixplugin/release/ -lrasterixplugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../rasterixplugin/debug/ -lrasterixplugin
else:unix: LIBS += -L$$OUT_PWD/../rasterixplugin/ -lrasterixplugin

INCLUDEPATH += $$PWD/../rasterixplugin
DEPENDPATH += $$PWD/../rasterixplugin

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../rasterixplugin/release/librasterixplugin.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../rasterixplugin/debug/librasterixplugin.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../rasterixplugin/release/rasterixplugin.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../rasterixplugin/debug/rasterixplugin.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../rasterixplugin/librasterixplugin.a
