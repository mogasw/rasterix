/****************************************************************************
**
** File: exporttool.h.
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
#ifndef EXPORTTOOL_H
#define EXPORTTOOL_H

#include "../rasterixplugin/tool.h"
#include "exportworker.h"

/*!
 * \brief The ExportTool class
 */
class ExportTool : public Rasterix::Tool
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.mogasw.Qt.Rasterix.Export" FILE "exportplugin.json")
    Q_INTERFACES(Rasterix::Tool)

public:
    ExportTool(QObject *parent = 0);
    virtual ~ExportTool() {}

    /* Rasterix tool interface */
    virtual QString name() override;
    virtual QString description() override;
    virtual QIcon icon() override;
    virtual QWidget* widget() override;
    virtual Rasterix::Worker* worker() override;
    virtual void initProcessingParams() override;
    virtual QList<QAction*> toolActions() override;
    virtual QToolBar* toolBar() override;

    void setDriverName(const QString &driverName);
    void setOutputFileName(const QString &outputFileName);

protected:
    QString _driverName;
    QString _inputFileName;
    QString _outputFileName;
    ExportWorker* _worker;
    QAction* _saveAction;
    QAction* _saveAsAction;
    QToolBar* _toolbar;
};

#endif // EXPORTTOOL_H
