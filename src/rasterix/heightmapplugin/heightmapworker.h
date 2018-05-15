/****************************************************************************
**
** File: heightmapworker.h.
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
#ifndef HEIGHTMAPWORKER_H
#define HEIGHTMAPWORKER_H

#include "../rasterixplugin/worker.h"
#include <QMap>
#include <QColor>

/*!
 * \brief The HeightMapWorker class
 */
class HeightMapWorker :
        public Rasterix::Worker
{
    Q_OBJECT
    Q_INTERFACES(Rasterix::Worker)
public:
    explicit HeightMapWorker(QObject *parent = nullptr);

    virtual ~HeightMapWorker() {}

    /* Rasterix::Worker interface */
    virtual void run() override;

    void setOutFolderPath(const QString &outFolderPath);
    void setDemAlgorithm(const QString &demAlgorithm);
    void setDemProcessingOptions(const QString &demProcessingOptions);
    void setTriProcessingOptions(const QString &triProcessingOptions);
    void setTpiProcessingOptions(const QString &tpiProcessingOptions);
    void setRoughnessProcessingOptions(const QString &roughnessProcessingOptions);
    void setColorReliefMap(const QMap<double, QColor> &colorReliefMap);

protected:
    QString _outFolderPath;
    QString _demAlgorithm;
    QString _demProcessingOptions;
    QString _triProcessingOptions;
    QString _tpiProcessingOptions;
    QString _roughnessProcessingOptions;
    QMap<double, QColor> _colorReliefMap;

    char** makeOptionList(QString opts);
};

#endif // HEIGHTMAPWORKER_H
