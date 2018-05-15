/****************************************************************************
**
** File: contourwidget.h.
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
#ifndef CONTOURWIDGET_H
#define CONTOURWIDGET_H

#include <QWidget>
#include <QVector>
#include <QListWidgetItem>
#include <QStandardItemModel>

namespace Ui {
class ContourWidget;
}

/*!
 * \brief The ContourWidget class
 */
class ContourWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContourWidget(QWidget *parent = 0);
    ~ContourWidget();

    void init(int nBands, double nodata);
    int getElevationBand() const;
    QString getElevationAttributeName() const;
    bool force3DVertex();
    bool ingnoreNodata();
    double getSkipValue();
    double getRegularInterval() const;
    QVector<double> getCustomIntervals() const;
    QString getOutFolder() const;

signals:
    void applyRequested();

private slots:
    void on_addLevButton_clicked();
    void on_delLevButton_clicked();
    void on_clearLevButton_clicked();
    void on_applyButton_clicked();

private:
    Ui::ContourWidget *ui;
    QStandardItemModel* _model;
    QString _outFolder;
};

#endif // CONTOURWIDGET_H
