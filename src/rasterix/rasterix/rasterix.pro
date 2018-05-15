#-------------------------------------------------
#
# Project created by QtCreator 2018-03-29T16:16:34
#
#-------------------------------------------------


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rasterix
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

RC_ICONS = rasterix.ico

macx {
    ICON = rasterix.icns
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    rasterview.cpp \
    aboutpluginsdialog.cpp \
    aboutdialog.cpp \
    creditsdialog.cpp

HEADERS += \
        mainwindow.h \
    rasterview.h \
    aboutpluginsdialog.h \
    aboutdialog.h \
    creditsdialog.h

FORMS += \
        mainwindow.ui \
    aboutpluginsdialog.ui \
    aboutdialog.ui \
    creditsdialog.ui


RESOURCES += \
    images.qrc



# STATIC PLUGINS
unix {
    LIBS    += ../../plugins/librasterix_inspector.a
    LIBS    += ../../plugins/librasterix_resize.a
    LIBS    += ../../plugins/librasterix_export.a
    LIBS    += ../../plugins/librasterix_reproject.a
    LIBS    += ../../plugins/librasterix_clip.a
    LIBS    += ../../plugins/librasterix_contour.a
    LIBS    += ../../plugins/librasterix_nodata.a
    LIBS    += ../../plugins/librasterix_heightmap.a
}

win32-g++ {
    LIBS    += ../../plugins/librasterix_inspector.a
    LIBS    += ../../plugins/librasterix_resize.a
    LIBS    += ../../plugins/librasterix_export.a
    LIBS    += ../../plugins/librasterix_reproject.a
    LIBS    += ../../plugins/librasterix_clip.a
    LIBS    += ../../plugins/librasterix_contour.a
    LIBS    += ../../plugins/librasterix_nodata.a
    LIBS    += ../../plugins/librasterix_heightmap.a
}

win32-msvc:CONFIG(debug, debug|release): {
    LIBS    += ../../plugins/rasterix_inspectord.lib
    LIBS    += ../../plugins/rasterix_resized.lib
    LIBS    += ../../plugins/rasterix_exportd.lib
    LIBS    += ../../plugins/rasterix_reprojectd.lib
    LIBS    += ../../plugins/rasterix_clipd.lib
    LIBS    += ../../plugins/rasterix_contourd.lib
    LIBS    += ../../plugins/rasterix_nodatad.lib
    LIBS    += ../../plugins/rasterix_heightmapd.lib
}

win32-msvc:CONFIG(release, debug|release): {
    LIBS    += ../../plugins/rasterix_inspector.lib
    LIBS    += ../../plugins/rasterix_resize.lib
    LIBS    += ../../plugins/rasterix_export.lib
    LIBS    += ../../plugins/rasterix_reproject.lib
    LIBS    += ../../plugins/rasterix_clip.lib
    LIBS    += ../../plugins/rasterix_contour.lib
    LIBS    += ../../plugins/rasterix_nodata.lib
    LIBS    += ../../plugins/rasterix_heightmap.lib
}

# SRS utils
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SRSutils/release/ -lSRSutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SRSutils/debug/ -lSRSutils
else:unix: LIBS += -L$$OUT_PWD/../SRSutils/ -lSRSutils

INCLUDEPATH += $$PWD/../SRSutils
DEPENDPATH += $$PWD/../SRSutils

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SRSutils/release/libSRSutils.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SRSutils/debug/libSRSutils.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SRSutils/release/SRSutils.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../SRSutils/debug/SRSutils.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../SRSutils/libSRSutils.a

# rasterixplugin
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


# GDAL INCLUDE
include(../common.pri)

DISTFILES += \
    stylesheet.qss
