/****************************************************************************
**
** File: srsdialog.h.
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
#ifndef SRSDIALOG_H
#define SRSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_spatialref.h>

namespace Ui {
class SRSDialog;
}

class SRSDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SRSDialog(QWidget *parent = 0);
    ~SRSDialog();

    OGRSpatialReference* getSelectedSRS();

private slots:
    void on_seachLineEdit_textChanged(const QString &text);
    void on_srsListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::SRSDialog *ui;

    void loadSRSList();
    void selectSRS(QListWidgetItem *item);
};

#endif // SRSDIALOG_H
