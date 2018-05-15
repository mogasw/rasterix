/****************************************************************************
**
** File: resizetool.cpp.
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
#include "resizetool.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief ResizeTool::ResizeTool
 * \param parent
 */
ResizeTool::ResizeTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new ResizeWidget();
    _worker = new ResizeWorker(this);
    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);

    connect(_widget, &ResizeWidget::applyRequested, this, &ResizeTool::process);
}

/*!
 * \brief ResizeTool::name
 * \return
 */
QString ResizeTool::name()
{
    return tr("Resize");
}

/*!
 * \brief ResizeTool::description
 * \return
 */
QString ResizeTool::description()
{
    return tr("Resize raster");
}

/*!
 * \brief ResizeTool::icon
 * \return
 */
QIcon ResizeTool::icon()
{
    return Reacticons::getIcon(u8"\uf065");
}

/*!
 * \brief ResizeTool::widget
 * \return
 */
QWidget *ResizeTool::widget()
{
    return _widget;
}

/*!
 * \brief ResizeTool::worker
 * \return
 */
Rasterix::Worker *ResizeTool::worker()
{
    return _worker;
}

/*!
 * \brief ResizeTool::initProcessingParams
 */
void ResizeTool::initProcessingParams()
{
    // setup worker
    _worker->setRaster(_inDataset);
    _worker->setResizeMode(_widget->getResolutionChecked() ? ResizeMode::ChangeResolution : ResizeMode::ChangeSize);
    _worker->setOutputSize(_widget->getSize());
    _worker->setOutputResolution(_widget->getWEResolution(), _widget->getNSResolution());
    _worker->setResamplingAlgorithm(_widget->getResampligAlgorithm());
}

/*!
 * \brief ResizeTool::toolActions
 * \return
 */
QList<QAction *> ResizeTool::toolActions()
{
    return QList<QAction*>() << _action;
}

/*!
 * \brief ResizeTool::initFromRaster
 */
void ResizeTool::initFromRaster()
{
    if (_inDataset != nullptr)
    {
        double adfGeoTransform[6];
        double weRes = 0.0, nsRes = 0.0;

        if(_inDataset->GetGeoTransform(adfGeoTransform) == CE_None)
        {
            weRes = adfGeoTransform[1];
            nsRes = adfGeoTransform[5] * -1.0; // ns res is a negative value
        }

        _widget->init(_inDataset->GetRasterXSize(),
                      _inDataset->GetRasterYSize(),
                      weRes, nsRes);
    }
}



