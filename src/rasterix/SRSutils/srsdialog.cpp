/****************************************************************************
**
** File: srsdialog.cpp.
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
#include "srsdialog.h"
#include "ui_srsdialog.h"

#include <QFile>

/*!
 * \brief SRSDialog::SRSDialog
 * \param parent
 */
SRSDialog::SRSDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SRSDialog)
{
    ui->setupUi(this);

    loadSRSList();
}

/*!
 * \brief SRSDialog::~SRSDialog
 */
SRSDialog::~SRSDialog()
{
    delete ui;
}

/*!
 * \brief SRSDialog::getSelectedSRS
 * \return
 */
OGRSpatialReference *SRSDialog::getSelectedSRS()
{
    OGRSpatialReference *result = new OGRSpatialReference;

    switch(ui->comboBox->currentIndex())
    {
    case 0:
    {
        if(ui->srsListWidget->currentItem() != nullptr)
        {
            bool parseOk = false;
            int epsgCode = ui->srsListWidget->currentItem()->data(Qt::UserRole).toInt(&parseOk);
            if(parseOk)
            {
                result->importFromEPSG(epsgCode);
            }
        }
    } break;

    case 1:
    {
        QString wktText = ui->wktTextEdit->toPlainText();
        const char *text = wktText.toStdString().data();
        result->importFromWkt((char**)&text);
    } break;

    case 2:
    {
        QString proj4Text = ui->proj4TextEdit->toPlainText();
        result->importFromProj4(proj4Text.toStdString().data());
    } break;
    }

    return result;
}

/*!
 * \brief SRSDialog::on_seachLineEdit_textChanged
 * \param arg1
 */
void SRSDialog::on_seachLineEdit_textChanged(const QString &text)
{
    ui->srsListWidget->clearSelection();

    QList<QListWidgetItem*> items = ui->srsListWidget->findItems(text, Qt::MatchContains | Qt::MatchWrap);
    if(items.length() > 0)
    {
        QListWidgetItem *item = items.first();

        item->setSelected(true);
        ui->srsListWidget->scrollToItem(item);
        selectSRS(item);
    }
}

/*!
 * \brief SRSDialog::loadSRSList
 */
void SRSDialog::loadSRSList()
{
    // disalbe default GDAL error handler
    // to suppress errors for missing SRSs
    CPLPushErrorHandler(CPLQuietErrorHandler);

    QFile epsgFile(":/epsg.txt");
    if(epsgFile.open(QIODevice::ReadOnly))
    {
        while(!epsgFile.atEnd())
        {
            QString line = epsgFile.readLine();
            int code = line.toInt();

            OGRSpatialReference srs;
            int result = srs.importFromEPSG(code);
            if(result == OGRERR_NONE)
            {
                const char* desc = nullptr;

                if(srs.IsProjected())
                {
                    desc = srs.GetAttrValue("PROJCS");
                }
                else
                {
                    desc = srs.GetAttrValue("GEOCS");
                }

                if(desc != nullptr)
                {
                    QListWidgetItem *item = new QListWidgetItem(QString(desc));
                    item->setData(Qt::UserRole, code);
                    ui->srsListWidget->addItem(item);
                }
            }
        }
        epsgFile.close();
    }

    // restore default error handler
    CPLPushErrorHandler(CPLDefaultErrorHandler);
}

/*!
 * \brief SRSDialog::selectSRS
 * \param item
 */
void SRSDialog::selectSRS(QListWidgetItem *item)
{
    bool parseOk = false;
    int epsgCode = item->data(Qt::UserRole).toInt(&parseOk);
    if(!parseOk)
        return;

    OGRSpatialReference srs;
    int result = srs.importFromEPSG(epsgCode);
    if(result == OGRERR_NONE)
    {
        const char* wkt = nullptr;
        srs.exportToWkt(const_cast<char**>(&wkt));

        ui->wktTextEdit->setPlainText(QString(wkt));

        const char* proj4text = nullptr;
        srs.exportToProj4(const_cast<char**>(&proj4text));

        ui->proj4TextEdit->setPlainText(QString(proj4text));
    }
}

/*!
 * \brief SRSDialog::on_srsListWidget_currentItemChanged
 * \param current
 * \param previous
 */
void SRSDialog::on_srsListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)

    selectSRS(current);
}
