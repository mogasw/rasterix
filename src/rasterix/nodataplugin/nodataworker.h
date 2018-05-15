/****************************************************************************
**
** File: nodataworker.h.
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
#ifndef NODATAWORKER_H
#define NODATAWORKER_H

#include "../rasterixplugin/worker.h"

/*!
 * \brief The NoDataFillMode enum
 */
enum class NoDataFillMode : int
{
    ByValue,
    Smoothing
};

/*!
 * \brief The NoDataWorker class
 */
class NoDataWorker : public Rasterix::Worker
{
    Q_OBJECT
    Q_INTERFACES(Rasterix::Worker)
public:
    explicit NoDataWorker(QObject *parent = nullptr);
    virtual ~NoDataWorker() {}

    /* Rasterix::Worker interface */
    virtual void run() override;

    void setMode(const NoDataFillMode &mode);
    void setNewNodataValue(double newNodataValue);
    void setMaxDistancePixels(double maxDistance);
    void setSmoothingPasses(int winLength);

protected:
    NoDataFillMode _mode;
    double _newNodataValue;
    double _maxDistancePixels;
    int _smoothingPasses;
};

#endif // NODATAWORKER_H
