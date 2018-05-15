/****************************************************************************
**
** File: nodatatool.cpp.
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
#include "nodatatool.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief NoDataTool::NoDataTool
 * \param parent
 */
NoDataTool::NoDataTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new NoDataWidget();
    _worker = new NoDataWorker(this);
    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);

    connect(_widget, &NoDataWidget::applyRequested, this, &NoDataTool::process);
}

/*!
 * \brief NoDataTool::name
 * \return
 */
QString NoDataTool::name()
{
    return tr("No data");
}

/*!
 * \brief NoDataTool::description
 * \return
 */
QString NoDataTool::description()
{
    return tr("Edit no data samples of a raster dataset");
}

/*!
 * \brief NoDataTool::icon
 * \return
 */
QIcon NoDataTool::icon()
{
    return Reacticons::getIcon("", "MaterialCommunityIcons");
}

/*!
 * \brief NoDataTool::widget
 * \return
 */
QWidget *NoDataTool::widget()
{
    return _widget;
}

/*!
 * \brief NoDataTool::worker
 * \return
 */
Rasterix::Worker *NoDataTool::worker()
{
    return _worker;
}

/*!
 * \brief NoDataTool::initProcessingParams
 */
void NoDataTool::initProcessingParams()
{
    // setup worker
    _worker->setRaster(_inDataset);
    _worker->setMode(_widget->isSmoothingChecked() ? NoDataFillMode::Smoothing : NoDataFillMode::ByValue);
    _worker->setNewNodataValue(_widget->getNewNoData());
    _worker->setMaxDistancePixels(_widget->getSmoothMaxDistance());
    _worker->setSmoothingPasses(_widget->getSmoothWinLenght());
}

QList<QAction *> NoDataTool::toolActions()
{
    return QList<QAction*>() << _action;
}

void NoDataTool::initFromRaster()
{
    if (_inDataset != nullptr)
    {
        double current_nd = 0;
        GDALRasterBand* first = _inDataset->GetRasterBand(1);
        if (first)
        {
             int ok = 0;
             current_nd = first->GetNoDataValue(&ok);

             if (!ok)
                 current_nd = -1; // no nodata...
        }

        _widget->init(current_nd);
    }
}

