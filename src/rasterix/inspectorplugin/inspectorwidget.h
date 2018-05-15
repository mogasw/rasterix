/****************************************************************************
**
** File: inspectorwidget.h.
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
#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QWidget>
#include <ogr_spatialref.h>

namespace Ui {
class InspectorWidget;
}

enum class CRSTextFormat : int
{
    RichText = 0,
    OpenGISWKT = 1,
    Proj4 = 2
};

class InspectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InspectorWidget(QWidget *parent = 0);
    ~InspectorWidget();

public slots:
    void setRasterPropertiesText(QString text);
    void setRasterCRS(OGRSpatialReference *crs);

private slots:
    void on_projModeComboBox_currentIndexChanged(int index);

    void on_openTextButton_clicked();

private:
    Ui::InspectorWidget *ui;
    OGRSpatialReference* _crs;
};

#endif // INSPECTORWIDGET_H
