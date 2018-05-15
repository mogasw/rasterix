/****************************************************************************
**
** File: aboutdialog.cpp.
** Created: 14/2018
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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "../reacticons/reacticons.h"
#include "creditsdialog.h"
#include <QDesktopServices>
#include <QUrl>

/*!
 * \brief AboutDialog::AboutDialog
 * \param parent
 */
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    this->setFixedSize(this->sizeHint());


    ui->versionLabel->setText(QString("%1: %2 (%3)").arg(tr("Version"), VER, VERSION_COMMIT));
    ui->closeButton->setIcon(Reacticons::getIcon(u8"\uf00d"));
}

/*!
 * \brief AboutDialog::~AboutDialog
 */
AboutDialog::~AboutDialog()
{
    delete ui;
}

/*!
 * \brief AboutDialog::on_websiteButton_clicked
 */
void AboutDialog::on_websiteButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/mogasw/rasterix"));
}

/*!
 * \brief AboutDialog::on_creditsButton_clicked
 */
void AboutDialog::on_creditsButton_clicked()
{
    CreditsDialog dialog;
    dialog.exec();
}
