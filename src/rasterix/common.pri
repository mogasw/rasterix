include(gdal.pri)

# BUILD VERSION based on git tags
GIT_VERSION = $$system(git --git-dir $$PWD/../../.git --work-tree $$PWD/../ describe --always --tags)
GIT_VERSION ~= s/-/"."
GIT_VERSION_PARTS = $$split(GIT_VERSION, ".")

VMAJOR = $$member(GIT_VERSION_PARTS, 0)
VMINOR = $$member(GIT_VERSION_PARTS, 1)
VBUILD = $$member(GIT_VERSION_PARTS, 2)
VCOMMIT = $$member(GIT_VERSION_PARTS, 3)

DEFINES += VERSION_MAJOR=$${VMAJOR}
DEFINES += VERSION_MINOR=$${VMINOR}
DEFINES += VERSION_BUILD=$${VBUILD}
DEFINES += VERSION_COMMIT='\\"$${VCOMMIT}\\"'

VERSTR = '\\"$${VMAJOR}.$${VMINOR}.$${VBUILD}\\"'
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string
VERSION = $${VMAJOR}.$${VMINOR}.$${VBUILD}

QMAKE_TARGET_COMPANY = Moga Software s.r.l.
QMAKE_TARGET_COPYRIGHT = Moga Software s.r.l.

# REACTICONS
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../reacticons/release/ -lreacticons
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../reacticons/debug/ -lreacticons
else:unix: LIBS += -L$$OUT_PWD/../reacticons/ -lreacticons

INCLUDEPATH += $$PWD/../reacticons
DEPENDPATH += $$PWD/../reacticons

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../reacticons/release/libreacticons.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../reacticons/debug/libreacticons.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../reacticons/release/reacticons.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../reacticons/debug/reacticons.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../reacticons/libreacticons.a

