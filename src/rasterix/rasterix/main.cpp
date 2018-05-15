/****************************************************************************
**
** File: aboutpluginsdialog.cpp.
** Created: 4/2018
**
**
** Copyright (C) 2018 Gabriella Giordano
** Copyright (C) 2018 Sergio Monteleone
** Copyright (C) 2018 Moga Software s.r.l.
** All rights reserved.
**
**
** This file is part of Rasterix.
** Rasterix is a GIS utility designed to process raster datasets.
**
** Rasterix is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** Rasterix is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Rasterix. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "mainwindow.h"
#include <QtPlugin>
#include <QApplication>
#include <QDir>
#include <QStyleFactory>
#include "../inspectorplugin/inspectortool.h"
#include "../resizeplugin/resizetool.h"
#include "../exportplugin/exporttool.h"
#include "../reprojectplugin/reprojecttool.h"
//#include "../clipplugin/cliptool.h"
#include "../contourplugin/contourtool.h"
#include "../nodataplugin/nodatatool.h"
#include "../heightmapplugin/heightmapcolortool.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "../reacticons/reacticons.h"

Q_IMPORT_PLUGIN(InspectorTool)
Q_IMPORT_PLUGIN(ResizeTool)
Q_IMPORT_PLUGIN(ExportTool)
Q_IMPORT_PLUGIN(ReprojectTool)
//Q_IMPORT_PLUGIN(ClipTool)
Q_IMPORT_PLUGIN(ContourTool)
Q_IMPORT_PLUGIN(NoDataTool)
Q_IMPORT_PLUGIN(HeightMapColorTool)

// THIS IS A WIP: need to remove read-only attribute on temporary folders.
void cleanupTemp()
{
    QString tempPath = QDir::tempPath();
    QDir tempDir(tempPath);
    QStringList subs = tempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (QString sub : subs)
    {
        if (sub.startsWith("rasterix", Qt::CaseInsensitive))
        {
            QDir subDir(sub);
            subDir.rmdir(".");
        }
    }
}

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    Q_INIT_RESOURCE(data);

    // init react icons
    Reacticons::init();

#ifdef Q_OS_WIN
    QFileInfo fInfo;
    fInfo.setFile(QString(argv[0]));
    QString dataPath("%1/data/");
    dataPath = dataPath.arg(fInfo.absolutePath());
    dataPath = QDir::toNativeSeparators(dataPath);
    CPLSetConfigOption("GDAL_DATA", dataPath.toStdString().c_str());
#endif

    // register gdal driver
    GDALAllRegister();

    // cleanup temporary dirs not deleted by chance...
    cleanupTemp();

    MainWindow w;
    w.show();
    return a.exec();
}
