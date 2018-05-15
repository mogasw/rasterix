/****************************************************************************
**
** File: nodatawidget.cpp.
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
#include "nodatawidget.h"
#include "ui_nodatawidget.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief NoDataWidget::NoDataWidget
 * \param parent
 */
NoDataWidget::NoDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoDataWidget)
{
    ui->setupUi(this);

    ui->applyButton->setIcon(Reacticons::getIcon(u8"\uf00c", "FontAwesome", QColor(Qt::green)));
    connect(ui->applyButton, &QPushButton::clicked, this, &NoDataWidget::applyRequested);
}

/*!
 * \brief NoDataWidget::~NoDataWidget
 */
NoDataWidget::~NoDataWidget()
{
    delete ui;
}

/*!
 * \brief NoDataWidget::init
 * \param current_nd
 */
void NoDataWidget::init(double current_nd)
{
    ui->currentNodataLabel->setText(QString::number(current_nd));
}

/*!
 * \brief NoDataWidget::getNewNoData
 * \return
 */
double NoDataWidget::getNewNoData() const
{
    return ui->newNodataSpinBox->value();
}

/*!
 * \brief NoDataWidget::getSmoothMaxDistance
 * \return
 */
double NoDataWidget::getSmoothMaxDistance() const
{
    return ui->maxDistanceSpinBox->value();
}

/*!
 * \brief NoDataWidget::getSmoothWinLenght
 * \return
 */
int NoDataWidget::getSmoothWinLenght() const
{
    return ui->winLenghtSpinBox->value();
}

/*!
 * \brief NoDataWidget::isSmoothingChecked
 * \return
 */
bool NoDataWidget::isSmoothingChecked() const
{
    return ui->smoothRadioButton->isChecked();
}
