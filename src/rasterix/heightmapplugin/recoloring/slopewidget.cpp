/****************************************************************************
**
** File: slopewidget.cpp.
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
#include "slopewidget.h"
#include "ui_slopewidget.h"

/*!
 * \brief SlopeWidget::SlopeWidget
 * \param parent
 */
SlopeWidget::SlopeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlopeWidget)
{
    ui->setupUi(this);
}

/*!
 * \brief SlopeWidget::~SlopeWidget
 */
SlopeWidget::~SlopeWidget()
{
    delete ui;
}

/*!
 * \brief SlopeWidget::getAlgorithm
 * \return
 */
QString SlopeWidget::getAlgorithm() const
{
    QString alg("ZevenbergenThorne");

    if (ui->algComboBox->currentIndex() > 0)
        alg = "Horn";

    return alg;
}

/*!
 * \brief SlopeWidget::usePercentValues
 * \return
 */
bool SlopeWidget::usePercentValues() const
{
    return ui->slopeValComboBox->currentText().contains("Percent");
}

/*!
 * \brief SlopeWidget::getSlopeElevationScalar
 * \return
 */
double SlopeWidget::getSlopeElevationScalar() const
{
    double scalar = 111120;

    if (ui->vunitComboBox->currentText().contains("Us Feet"))
        scalar = 370400;

    return scalar;
}
