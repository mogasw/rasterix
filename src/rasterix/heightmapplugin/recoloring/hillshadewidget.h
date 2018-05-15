/****************************************************************************
**
** File: hillshadewidget.h.
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
#ifndef HILLSHADEWIDGET_H
#define HILLSHADEWIDGET_H

#include <QWidget>

namespace Ui {
class HillshadeWidget;
}

/*!
 * \brief The HillshadeWidget class
 */
class HillshadeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HillshadeWidget(QWidget *parent = 0);
    ~HillshadeWidget();

    QString getAlgorithm() const;
    bool useMultidirectionalShading() const;
    bool userDefinedShading() const;
    double getLightAzimuth() const;
    double getLightAltitude() const;
    bool useCombinedShading() const;
    double getVerticalExaggeration() const;
    double getHillshadeElevationScalar() const;

private:
    Ui::HillshadeWidget *ui;
};

#endif // HILLSHADEWIDGET_H
