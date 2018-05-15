/****************************************************************************
**
** File: contourworker.h.
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
#ifndef CONTOURWORKER_H
#define CONTOURWORKER_H

#include "../rasterixplugin/worker.h"
#include <QVector>

/*!
 * \brief The ContourWorker class
 */
class ContourWorker : public Rasterix::Worker
{
    Q_OBJECT
    Q_INTERFACES(Rasterix::Worker)

public:
    explicit ContourWorker(QObject *parent = nullptr);
    virtual ~ContourWorker() {}

    /* Rasterix::Worker interface */
    virtual void run() override;

    void setOutFolderPath(const QString &outFolderPath);
    void setBandIndex(int bandIndex);
    void setBaseLevel(double baseLevel);
    void setRegularInterval(double regularInterval);
    void setCustomIntervals(const QVector<double> &customIntervals);
    void setIgnoreNodata(bool ignoreNodata);
    void setSkipValue(double skipValue);
    void setElevationAttrName(const QString &elevationAttrName);
    void set3D(bool threeD);

signals:

public slots:

protected:
    QString _outFolderPath;
    int _bandIndex;
    double _baseLevel;
    double _regularInterval;
    QVector<double> _customIntervals;
    bool _3D;
    bool _ignoreNodata;
    double _skipValue;
    QString _elevationAttrName;
};

#endif // CONTOURWORKER_H
