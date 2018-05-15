/****************************************************************************
**
** File: aboutpluginsdialog.cpp.
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
#include "aboutpluginsdialog.h"
#include "ui_aboutpluginsdialog.h"
#include "../reacticons/reacticons.h"
#include <QJsonObject>

AboutPluginsDialog::AboutPluginsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPluginsDialog)
{
    ui->setupUi(this);

    setFixedSize(this->minimumSize());
    ui->closeButton->setIcon(Reacticons::getIcon(u8"\uf00d"));
}

AboutPluginsDialog::~AboutPluginsDialog()
{
    delete ui;
}

void AboutPluginsDialog::init(QList<Rasterix::Tool *> tools)
{
    ui->pluginsTableWidget->clear();
    ui->pluginsTableWidget->setRowCount(tools.length());
    ui->pluginsTableWidget->setColumnCount(3);
    ui->pluginsTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Version") << tr("Description"));

    int i=0;
    for (Rasterix::Tool* tool : tools)
    {
        if (tool)
        {
            QJsonObject json = tool->metadata();
            QJsonObject metadata = json["MetaData"].toObject();
            ui->pluginsTableWidget->setItem(i, 0, new QTableWidgetItem(metadata["Name"].toString()));
            ui->pluginsTableWidget->setItem(i, 1, new QTableWidgetItem(metadata["Version"].toString()));
            ui->pluginsTableWidget->setItem(i, 2, new QTableWidgetItem(metadata["Description"].toString()));
        }
        i++;
    }
}
