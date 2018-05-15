/****************************************************************************
**
** File: inspectortool.cpp.
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
#include "inspectortool.h"
#include "gdal.h"
#include "gdal_priv.h"
#include <ogr_spatialref.h>

#include "../reacticons/reacticons.h"

InspectorTool::InspectorTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new InspectorWidget();
    _raster = nullptr;

    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);
}

QString InspectorTool::name()
{
    return tr("Inspect");
}

QString InspectorTool::description()
{
    return tr("Show raster file properties");
}

QIcon InspectorTool::icon()
{
    return Reacticons::getIcon(u8"\uf05a");
}

QWidget *InspectorTool::widget()
{
    return _widget;
}

void InspectorTool::setRaster(GDALDataset* dataset)
{
    _raster = (GDALDataset*) dataset;

    if (_raster != nullptr)
    {
        // show file properties
        QString properties("Layers: %1\n" \
                           "Width: %2 px\n" \
                           "Height: %3 px\n" \
                           "Bands: %4\n" \
                           "Driver: %5"
                           );

        properties = properties.arg(
                    QString::number(_raster->GetLayerCount()),
                    QString::number(_raster->GetRasterXSize()),
                    QString::number(_raster->GetRasterYSize()),
                    QString::number(_raster->GetRasterCount()),
                    QString(_raster->GetDriverName()));

        _widget->setRasterPropertiesText(properties);


        const char* data = _raster->GetProjectionRef();
        _widget->setRasterCRS(new OGRSpatialReference(data));
    }
}

Rasterix::Worker *InspectorTool::worker()
{
    return nullptr;
}

QList<QAction *> InspectorTool::toolActions()
{
    return QList<QAction*>() << _action;
}



