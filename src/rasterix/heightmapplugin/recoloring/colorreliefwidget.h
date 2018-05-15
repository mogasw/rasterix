/****************************************************************************
**
** File: colorreliefwidget.h.
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
#ifndef COLORRELIEFWIDGET_H
#define COLORRELIEFWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QColor>

namespace Ui {
class ColorReliefWidget;
}

/*!
 * \brief The ColorReliefWidget class
 */
class ColorReliefWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorReliefWidget(QWidget *parent = 0);
    ~ColorReliefWidget();

    bool useTransparency() const;
    QMap<double, QColor> getLevelsMap() const;

private slots:
    void on_addButton_clicked();
    void on_delButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::ColorReliefWidget *ui;
    QStandardItemModel* _model;

    QColor nextRandomColor() const;
};

#endif // COLORRELIEFWIDGET_H
