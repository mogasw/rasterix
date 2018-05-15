/****************************************************************************
**
** File: contourtool.cpp.
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
#include "contourtool.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief ContourTool::ContourTool
 * \param parent
 */
ContourTool::ContourTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new ContourWidget();
    _worker = new ContourWorker(this);
    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);

    connect(_widget, &ContourWidget::applyRequested, this, &ContourTool::process);
}

/*!
 * \brief ContourTool::name
 * \return
 */
QString ContourTool::name()
{
    return tr("Contour");
}

/*!
 * \brief ContourTool::description
 * \return
 */
QString ContourTool::description()
{
    return tr("Extract contours from raster");
}

/*!
 * \brief ContourTool::icon
 * \return
 */
QIcon ContourTool::icon()
{
    return Reacticons::getIcon(u8"\uf201");
}

/*!
 * \brief ContourTool::widget
 * \return
 */
QWidget *ContourTool::widget()
{
    return _widget;
}

/*!
 * \brief ContourTool::worker
 * \return
 */
Rasterix::Worker *ContourTool::worker()
{
    return _worker;
}

/*!
 * \brief ContourTool::initProcessingParams
 */
void ContourTool::initProcessingParams()
{
    _worker->setRaster(_inDataset);
    _worker->setOutFolderPath(_widget->getOutFolder());
    _worker->setBandIndex(_widget->getElevationBand());
    _worker->setBaseLevel(0);
    _worker->setRegularInterval(_widget->getRegularInterval());
    _worker->setCustomIntervals(_widget->getCustomIntervals());
    _worker->set3D(_widget->force3DVertex());
    _worker->setIgnoreNodata(_widget->ingnoreNodata());
    _worker->setSkipValue(_widget->getSkipValue());
    _worker->setElevationAttrName(_widget->getElevationAttributeName());
}

/*!
 * \brief ContourTool::toolActions
 * \return
 */
QList<QAction *> ContourTool::toolActions()
{
    return QList<QAction*>() << _action;
}

/*!
 * \brief ContourTool::initFromRaster
 */
void ContourTool::initFromRaster()
{
    if (_inDataset != nullptr)
    {
        double nodata = -1.0;
        int nBands = _inDataset->GetRasterCount();

        if (nBands > 0)
            nodata = _inDataset->GetRasterBand(1)->GetNoDataValue();

        _widget->init(nBands, nodata);
    }
}
