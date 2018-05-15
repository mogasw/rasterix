/****************************************************************************
**
** File: inspectorwidget.cpp.
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
#include "inspectorwidget.h"
#include "ui_inspectorwidget.h"
#include "cpl_conv.h"

InspectorWidget::InspectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InspectorWidget)
{
    ui->setupUi(this);
    _crs = nullptr;

    // TODO: add more formats
    ui->projModeComboBox->insertItem((int) CRSTextFormat::OpenGISWKT, tr("WKT"));
    ui->projModeComboBox->insertItem((int) CRSTextFormat::Proj4, tr("Proj4"));
}

InspectorWidget::~InspectorWidget()
{
    delete ui;
}

void InspectorWidget::setRasterPropertiesText(QString text)
{
    ui->propertiesTextEdit->setText(text);
}

void InspectorWidget::setRasterCRS(OGRSpatialReference* crs)
{
    if (_crs != nullptr)
        OGRSpatialReference::DestroySpatialReference(_crs);

    _crs = crs;

    ui->projModeComboBox->setCurrentIndex((int) CRSTextFormat::OpenGISWKT);
}

void InspectorWidget::on_projModeComboBox_currentIndexChanged(int index)
{
    if (_crs != nullptr && (_crs->Validate() == OGRERR_NONE))
    {
        CRSTextFormat format = (CRSTextFormat) index;

        const char* data = nullptr;

        switch (format)
        {
        case CRSTextFormat::OpenGISWKT:
            _crs->exportToPrettyWkt(const_cast<char**>(&data));
            break;

        case CRSTextFormat::Proj4:
            _crs->exportToProj4(const_cast<char**>(&data));
            break;

        default:
            // todo rich text
            _crs->exportToWkt(const_cast<char**>(&data));
            break;
        }

        if (data != nullptr)
        {
            ui->projectionTextEdit->setText(QString(data));
        }
    }
}

void InspectorWidget::on_openTextButton_clicked()
{
    QString text = ui->projectionTextEdit->toPlainText();
    if (!text.isEmpty())
    {
        QTextEdit* bigEdit = new QTextEdit();
        bigEdit->setAttribute(Qt::WA_DeleteOnClose);
        bigEdit->setText(text);
        bigEdit->setWindowModality(Qt::ApplicationModal);
        bigEdit->setReadOnly(true);
        bigEdit->show();
    }
}
