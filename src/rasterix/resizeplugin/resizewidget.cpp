/****************************************************************************
**
** File: resizewidget.cpp.
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
#include "resizewidget.h"
#include "ui_resizewidget.h"
#include "../reacticons/reacticons.h"

ResizeWidget::ResizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResizeWidget)
{
    ui->setupUi(this);
    _originalRatio = 1.0f;

    ui->applyButton->setIcon(Reacticons::getIcon(u8"\uf00c", "FontAwesome", QColor(Qt::green)));
    connect(ui->applyButton, &QPushButton::clicked, this, &ResizeWidget::applyRequested);
}

ResizeWidget::~ResizeWidget()
{
    delete ui;
}

void ResizeWidget::init(int pxWidth, int pxHeight, double weRes, double nsRes)
{
    ui->widthSpinBox->blockSignals(true);
    ui->widthSpinBox->setValue(pxWidth);
    ui->widthSpinBox->blockSignals(false);

    ui->heigthSpinBox->blockSignals(true);
    ui->heigthSpinBox->setValue(pxHeight);
    ui->heigthSpinBox->blockSignals(false);

    ui->weResSpinBox->setValue(weRes);
    ui->nsResSpinBox->setValue(nsRes);
    _originalRatio = pxHeight / pxWidth;
}

QSize ResizeWidget::getSize() const
{
    return QSize(ui->widthSpinBox->value(), ui->heigthSpinBox->value());
}

double ResizeWidget::getWEResolution() const
{
    return ui->weResSpinBox->value();
}

double ResizeWidget::getNSResolution() const
{
    return ui->nsResSpinBox->value();
}

int ResizeWidget::getResampligAlgorithm() const
{
    return ui->resamplingComboBox->currentIndex();
}

bool ResizeWidget::getResolutionChecked() const
{
    return ui->resolutionRadioButton->isChecked();
}

void ResizeWidget::on_widthSpinBox_valueChanged(int arg1)
{
    if (ui->ratioButton->isChecked())
    {
        // update height keeping ratio
        ui->heigthSpinBox->blockSignals(true);
        ui->heigthSpinBox->setValue(arg1 * _originalRatio);
        ui->heigthSpinBox->blockSignals(false);
    }
}

void ResizeWidget::on_heigthSpinBox_valueChanged(int arg1)
{
    if (ui->ratioButton->isChecked())
    {
        // update width keeping ratio
        ui->widthSpinBox->blockSignals(true);
        ui->widthSpinBox->setValue(arg1 * _originalRatio);
        ui->widthSpinBox->blockSignals(false);
    }
}

void ResizeWidget::on_ratioButton_toggled(bool checked)
{
    if (checked)
    {
        on_widthSpinBox_valueChanged(ui->widthSpinBox->value());
    }
}
