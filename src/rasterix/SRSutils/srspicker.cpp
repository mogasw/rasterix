/****************************************************************************
**
** File: srspicker.cpp.
** Created: 5/2018
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
#include "srspicker.h"
#include "ui_srspicker.h"

/*!
 * \brief SRSPicker::SRSPicker
 * \param parent
 */
SRSPicker::SRSPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SRSPicker)
{
    ui->setupUi(this);

    srsDialog = new SRSDialog(this);
}

/*!
 * \brief SRSPicker::~SRSPicker
 */
SRSPicker::~SRSPicker()
{
    delete ui;
}

/*!
 * \brief SRSPicker::getSelectedSRS
 * \return
 */
OGRSpatialReference *SRSPicker::getSelectedSRS()
{
    return srsDialog->getSelectedSRS();
}

/*!
 * \brief SRSPicker::on_toolButton_clicked
 */
void SRSPicker::on_toolButton_clicked()
{
    srsDialog->exec();

    OGRSpatialReference *srs = srsDialog->getSelectedSRS();
    if(srs != nullptr)
    {
        const char* desc = nullptr;
        if(srs->IsProjected())
        {
            desc = srs->GetAttrValue("PROJCS");
        }
        else
        {
            desc = srs->GetAttrValue("GEOCS");
        }
        if(desc != nullptr)
        {
            ui->label->setText(QString(desc));
        }
    }
}
