# GDAL LIBRARY
#unix:!macx {
#    LIBS += -L/opt/gdal/lib/ -lgdal -lexpat -lz -ltiff -lproj -ljasper -lgeos_c -lkmlbase -lkmldom -lkmlengine -lkmlxsd -lkmlregionator

#    INCLUDEPATH += /opt/gdal/include
#    DEPENDPATH += /opt/gdal/include

#    PRE_TARGETDEPS += /opt/gdal/lib/libgdal.a
#}

unix:!macx {
    LIBS += -lgdal

    INCLUDEPATH += /usr/include/gdal
    DEPENDPATH += /usr/include/gdal
}

win32-g++ {
    LIBS += -L/opt/gdal-mingw/lib/ -lgdal -lexpat -lz -ltiff -lproj -ljasper -lodbc32 -lodbccp32 -lpsapi

# workaround for MINGW64 locale_t definition
    DEFINES += locale_t=_locale_t

    INCLUDEPATH += /opt/gdal-mingw/include
    DEPENDPATH += /opt/gdal-mingw/include

    PRE_TARGETDEPS += /opt/gdal-mingw/lib/libgdal.a
}

win32-msvc {
    GDAL_PATH="C:/development/libs/gdal/gdal-2.3.0/build"

    LIBS += $${GDAL_PATH}/lib/gdal_i.lib

    INCLUDEPATH += $${GDAL_PATH}/include
    DEPENDPATH += $${GDAL_PATH}/include

    PRE_TARGETDEPS += $${GDAL_PATH}/lib/gdal_i.lib
}


mac {
    LIBS += -framework IOKit
    LIBS += -framework CoreFoundation

    QMAKE_LFLAGS += -F/Library/Frameworks/

    LIBS += -framework GDAL
    LIBS += -framework PROJ
    LIBS += -framework GEOS

    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Headers
    INCLUDEPATH += /Library/Frameworks/PROJ.framework/Headers
    INCLUDEPATH += /Library/Frameworks/GEOS.framework/Headers
}

