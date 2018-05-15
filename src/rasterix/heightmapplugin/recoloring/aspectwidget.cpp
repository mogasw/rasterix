/****************************************************************************
**
** File: aspectwidget.cpp.
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
#include "aspectwidget.h"
#include "ui_aspectwidget.h"

/*!
 * \brief AspectWidget::AspectWidget
 * \param parent
 */
AspectWidget::AspectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AspectWidget)
{
    ui->setupUi(this);
}

/*!
 * \brief AspectWidget::~AspectWidget
 */
AspectWidget::~AspectWidget()
{
    delete ui;
}

/*!
 * \brief AspectWidget::getAlgorithm
 * \return
 */
QString AspectWidget::getAlgorithm() const
{
    QString alg("ZevenbergenThorne");

    if (ui->algComboBox->currentIndex() > 0)
        alg = "Horn";

    return alg;
}

/*!
 * \brief AspectWidget::useAzimuthReferredValues
 * \return
 */
bool AspectWidget::useAzimuthReferredValues() const
{
    return ui->angleRefComboBox->currentText().contains("Azimuth");
}

/*!
 * \brief AspectWidget::useZeroForFlatAreas
 * \return
 */
bool AspectWidget::useZeroForFlatAreas() const
{
    return ui->flatComboBox->currentText().startsWith("0");
}
