/****************************************************************************
**
** File: reprojecttool.cpp.
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
#include "reprojecttool.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief ReprojectTool::ReprojectTool
 * \param parent
 */
ReprojectTool::ReprojectTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new ReprojectWidget();
    _worker = new ReprojectWorker(this);
    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);

    connect(_widget, &ReprojectWidget::applyRequested, this, &ReprojectTool::process);
}

/*!
 * \brief ReprojectTool::name
 * \return
 */
QString ReprojectTool::name()
{
    return tr("Reproject");
}

/*!
 * \brief ReprojectTool::description
 * \return
 */
QString ReprojectTool::description()
{
    return tr("Reproject raster");
}

/*!
 * \brief ReprojectTool::icon
 * \return
 */
QIcon ReprojectTool::icon()
{
    return Reacticons::getIcon(u8"\uf0ac");
}

/*!
 * \brief ReprojectTool::widget
 * \return
 */
QWidget *ReprojectTool::widget()
{
    return _widget;
}

/*!
 * \brief ReprojectTool::worker
 * \return
 */
Rasterix::Worker* ReprojectTool::worker()
{
    return _worker;
}

/*!
 * \brief ReprojectTool::initProcessingParams
 */
void ReprojectTool::initProcessingParams()
{
    _worker->setRaster(_inDataset);
    _worker->setTargetSRS(_widget->getTargetSRS());
}

/*!
 * \brief ReprojectTool::toolActions
 * \return
 */
QList<QAction *> ReprojectTool::toolActions()
{
    return QList<QAction *>() << _action;
}

/*!
 * \brief ReprojectTool::initFromRaster
 */
void ReprojectTool::initFromRaster()
{
    // TODO: populate widget and tool metadata after setRaster() is invoked.
}
