/****************************************************************************
**
** File: resizeworker.h.
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
#ifndef RESIZEWORKER_H
#define RESIZEWORKER_H

#include "../rasterixplugin/worker.h"
#include "gdal.h"
#include "gdal_priv.h"
#include <QSize>

/*!
 * \brief The ResizeMode enum
 */
enum class ResizeMode : int
{
    ChangeSize,
    ChangeResolution
};

/*!
 * \brief The ResizeWorker class
 */
class ResizeWorker :
        public Rasterix::Worker
{
    Q_OBJECT
    Q_INTERFACES(Rasterix::Worker)

public:
    ResizeWorker(QObject* parent = nullptr);
    virtual ~ResizeWorker() {}

    /* Rasterix::Worker interface */
    virtual void run() override;

    void setResizeMode(ResizeMode mode);
    void setOutputSize(const QSize &outputSize);
    void setOutputResolution(double weRes, double nsRes);
    void setResamplingAlgorithm(int resamplingAlgorithm);

protected:
    ResizeMode _mode;
    QSize _outputSize;
    double _wePixelSizeMeters;
    double _nsPixelSizeMeters;
    GDALRIOResampleAlg _resamplingAlgorithm;
};

#endif // RESIZEWORKER_H
