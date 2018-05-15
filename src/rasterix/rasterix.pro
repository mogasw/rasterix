TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    reacticons \
    SRSutils \
    rasterixplugin \
    inspectorplugin \
    resizeplugin \
    exportplugin \
    reprojectplugin \
    clipplugin \
    contourplugin \
    nodataplugin \
    heightmapplugin \
    rasterix

# subproject paths
inspectorplugin.subdir = inspectorplugin
rasterizplugin.subdir = rasterizplugin
exportplugin.subdir = exportplugin
reprojectplugin.subdir = reprojectplugin
clipplugin.subdir = clipplugin
contourplugin.subdir = contourplugin
nodataplugin.subdir = nodataplugin
heightmapplugin.subdir = heightmapplugin

# depencencies
inspectorplugin.depends = rasterixplugin
rasterizplugin.depends = rasterixplugin
exportplugin.depends = rasterixplugin
reprojectplugin.depends = rasterixplugin
clipplugin.depends = rasterixplugin
contourplugin.depends = rasterixplugin
nodataplugin.depends = rasterixplugin
heightmapplugin.depends = rasterixplugin

rasterix.depends = \
    SRSutils \
    reacticons \
    rasterixplugin \
    inspectorplugin \
    resizeplugin \
    exportplugin \
    reprojectplugin \
    clipplugin \
    contourplugin \
    nodataplugin \
    heightmapplugin
