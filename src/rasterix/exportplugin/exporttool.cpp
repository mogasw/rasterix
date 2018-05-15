/****************************************************************************
**
** File: exporttool.cpp.
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
#include "exporttool.h"
#include "gdal.h"
#include "gdal_priv.h"

#include "../reacticons/reacticons.h"

/*!
 * \brief ExportTool::ExportTool
 * \param parent
 */
ExportTool::ExportTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _worker = new ExportWorker(this);

    _saveAction = new QAction(this);
    _saveAction->setData(QVariant::fromValue((void*) this));
    _saveAction->setIcon(Reacticons::getIcon(""));
    _saveAction->setText(name());
    _saveAction->setToolTip(description());

    _saveAsAction = new QAction(this);
    _saveAsAction->setData(QVariant::fromValue((void*) this));
    _saveAsAction->setIcon(Reacticons::getIcon(""));
    _saveAsAction->setText(tr("Save as"));
    _saveAsAction->setToolTip(description());

    _toolbar = new QToolBar();
    _toolbar->addAction(_saveAction);
    _toolbar->addAction(_saveAsAction);
}

/*!
 * \brief ExportTool::name
 * \return
 */
QString ExportTool::name()
{
    return tr("Save");
}

/*!
 * \brief ExportTool::description
 * \return
 */
QString ExportTool::description()
{
    return tr("Save raster file");
}

/*!
 * \brief ExportTool::icon
 * \return
 */
QIcon ExportTool::icon()
{
    return Reacticons::getIcon("\f20c", "Foundation");
}

/*!
 * \brief ExportTool::widget
 * \return
 */
QWidget *ExportTool::widget()
{
    return nullptr;
}

/*!
 * \brief ExportTool::worker
 * \return
 */
Rasterix::Worker *ExportTool::worker()
{
    return _worker;
}

/*!
 * \brief ExportTool::initProcessingParams
 */
void ExportTool::initProcessingParams()
{
    _worker->setRaster(_inDataset);
    _worker->setOutputFileName(_outputFileName);
    _worker->setDriverName(_driverName);
}

/*!
 * \brief ExportTool::toolActions
 * \return
 */
QList<QAction *> ExportTool::toolActions()
{
    return QList<QAction*>() << _saveAction << _saveAsAction;
}

/*!
 * \brief ExportTool::toolBar
 * \return
 */
QToolBar *ExportTool::toolBar()
{
    return _toolbar;
}

/*!
 * \brief ExportTool::setDriverName
 * \param driverName
 */
void ExportTool::setDriverName(const QString &driverName)
{
    _driverName = driverName;
}

/*!
 * \brief ExportTool::setOutputFileName
 * \param outputFileName
 */
void ExportTool::setOutputFileName(const QString &outputFileName)
{
    _outputFileName = outputFileName;
}


