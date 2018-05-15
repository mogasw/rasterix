/****************************************************************************
**
** File: hillshadewidget.cpp.
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
#include "hillshadewidget.h"
#include "ui_hillshadewidget.h"

/*!
 * \brief HillshadeWidget::HillshadeWidget
 * \param parent
 */
HillshadeWidget::HillshadeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HillshadeWidget)
{
    ui->setupUi(this);
}

/*!
 * \brief HillshadeWidget::~HillshadeWidget
 */
HillshadeWidget::~HillshadeWidget()
{
    delete ui;
}

/*!
 * \brief HillshadeWidget::getAlgorithm
 * \return
 */
QString HillshadeWidget::getAlgorithm() const
{
   QString alg("ZevenbergenThorne");

   if (ui->algComboBox->currentIndex() > 0)
       alg = "Horn";

   return alg;
}

/*!
 * \brief HillshadeWidget::useMultidirectionalShading
 * \return
 */
bool HillshadeWidget::useMultidirectionalShading() const
{
    return ui->multiDirectionRadioButton->isChecked();
}

/*!
 * \brief HillshadeWidget::userDefinedShading
 * \return
 */
bool HillshadeWidget::userDefinedShading() const
{
    return ui->userDefRadioButton->isChecked();
}

/*!
 * \brief HillshadeWidget::getLightAzimuth
 * \return
 */
double HillshadeWidget::getLightAzimuth() const
{
    return ui->azimuthSpinBox->value();
}

/*!
 * \brief HillshadeWidget::getLightAltitude
 * \return
 */
double HillshadeWidget::getLightAltitude() const
{
    return ui->altitudeSpinBox->value();
}

/*!
 * \brief HillshadeWidget::combineShading
 * \return
 */
bool HillshadeWidget::useCombinedShading() const
{
    return ui->combinedCheckBox->isChecked();
}

/*!
 * \brief HillshadeWidget::getVerticalExaggeration
 * \return
 */
double HillshadeWidget::getVerticalExaggeration() const
{
    double vexa = 1.0;

    if (ui->vexaCheckBox->isChecked())
        vexa = ui->vexaSpinBox->value();

    return vexa;
}

/*!
 * \brief HillshadeWidget::getHillshadeElevationScalar
 * \return
 */
double HillshadeWidget::getHillshadeElevationScalar() const
{
    double scalar = 111120;

    if (ui->vunitComboBox->currentText().contains("Us Feet"))
        scalar = 370400;

    return scalar;
}
