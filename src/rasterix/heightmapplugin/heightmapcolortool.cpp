/****************************************************************************
**
** File: heightmapcolortool.cpp.
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
#include "heightmapcolortool.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief HeightMapColorTool::HeightMapColorTool
 * \param parent
 */
HeightMapColorTool::HeightMapColorTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new HeightMapWidget();
    _worker = new HeightMapWorker(this);
    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);

    connect(_widget, &HeightMapWidget::applyRequested, this, &HeightMapColorTool::process);
}

/*!
 * \brief HeightMapColorTool::name
 * \return
 */
QString HeightMapColorTool::name()
{
    return tr("HeightMap");
}

/*!
 * \brief HeightMapColorTool::description
 * \return
 */
QString HeightMapColorTool::description()
{
    return tr("Set the color table of a raster dataset with an elevation attribute");
}

/*!
 * \brief HeightMapColorTool::icon
 * \return
 */
QIcon HeightMapColorTool::icon()
{
    return Reacticons::getIcon(u8"\uf1fe");
}

/*!
 * \brief HeightMapColorTool::widget
 * \return
 */
QWidget *HeightMapColorTool::widget()
{
    return _widget;
}

/*!
 * \brief HeightMapColorTool::worker
 * \return
 */
Rasterix::Worker *HeightMapColorTool::worker()
{
    return _worker;
}

/*!
 * \brief HeightMapColorTool::initProcessingParams
 */
void HeightMapColorTool::initProcessingParams()
{
    _worker->setRaster(_inDataset);
    _worker->setOutFolderPath(_widget->getOutFolder());
    _worker->setDemAlgorithm(_widget->getDEMAlgorithm());
    _worker->setDemProcessingOptions(_widget->getDEMProcessingOptionString());
    _worker->setTriProcessingOptions(_widget->getTRIProcessingOptionString());
    _worker->setTpiProcessingOptions(_widget->getTPIProcessingOptionString());
    _worker->setRoughnessProcessingOptions(_widget->getRoughnessProcessingOptionString());
    _worker->setColorReliefMap(_widget->getLevelColorMap());
}

QList<QAction *> HeightMapColorTool::toolActions()
{
    return QList<QAction*>() << _action;
}

void HeightMapColorTool::initFromRaster()
{
    if (_inDataset)
    {
        _widget->setBandCount(_inDataset->GetRasterCount());
    }
}
