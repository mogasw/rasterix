/****************************************************************************
**
** File: heightmapwidget.h.
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
#ifndef HEIGHTMAPWIDGET_H
#define HEIGHTMAPWIDGET_H

#include <QWidget>
#include <QMap>
#include <QColor>

namespace Ui {
class HeightMapWidget;
}

class HeightMapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeightMapWidget(QWidget *parent = 0);
    ~HeightMapWidget();

    int getElevationBand() const;
    bool computeEdges() const;

    void setBandCount(int bands);

    QString getDEMAlgorithm() const;
    QString getDEMProcessingOptionString() const;
    QString getTRIProcessingOptionString() const;
    QString getTPIProcessingOptionString() const;
    QString getRoughnessProcessingOptionString() const;

    /* color relief */
    QMap<double, QColor> getLevelColorMap();

    QString getOutFolder() const;

signals:
    void applyRequested();

private slots:
    void on_colorModeComboBox_currentIndexChanged(int index);
    void on_applyButton_clicked();

private:
    Ui::HeightMapWidget *ui;
    QString _outFolder;
};

#endif // HEIGHTMAPWIDGET_H
