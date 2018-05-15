/****************************************************************************
**
** File: heightmapwidget.cpp.
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
#include "heightmapwidget.h"
#include "ui_heightmapwidget.h"
#include "recoloring/hillshadewidget.h"
#include "recoloring/slopewidget.h"
#include "recoloring/aspectwidget.h"
#include "recoloring/colorreliefwidget.h"
#include "../reacticons/reacticons.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>

/*!
 * \brief HeightMapWidget::HeightMapWidget
 * \param parent
 */
HeightMapWidget::HeightMapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeightMapWidget)
{
    ui->setupUi(this);

    ui->applyButton->setIcon(Reacticons::getIcon(u8"\uf00c", "FontAwesome", QColor(Qt::green)));
    on_colorModeComboBox_currentIndexChanged(0);
}

/*!
 * \brief HeightMapWidget::~HeightMapWidget
 */
HeightMapWidget::~HeightMapWidget()
{
    delete ui;
}

/*!
 * \brief HeightMapWidget::getElevationBand
 * \return
 */
int HeightMapWidget::getElevationBand() const
{
    return ui->bandSpinBox->value();
}

/*!
 * \brief HeightMapWidget::computeEdges
 * \return
 */
bool HeightMapWidget::computeEdges() const
{
    return ui->edgesCheckBox->isChecked();
}

/*!
 * \brief HeightMapWidget::setBandCount
 * \param bands
 */
void HeightMapWidget::setBandCount(int bands)
{
    ui->bandSpinBox->setMaximum(bands);
}

/*!
 * \brief HeightMapWidget::getDEMAlgorithm
 * \return
 */
QString HeightMapWidget::getDEMAlgorithm() const
{
    QString alg;

    int index = ui->colorModeComboBox->currentIndex();
    switch (index) {
    case 0:
        alg = "hillshade";
        break;
    case 1:
        alg = "slope";
        break;
    case 2:
        alg = "aspect";
        break;
    case 3:
        alg = "color-relief";
        break;
    default:
        break;
    }

    return alg;
}

/*!
 * \brief HeightMapWidget::getDEMProcessingOptionString
 * \return
 */
QString HeightMapWidget::getDEMProcessingOptionString() const
{
    QString params;

    if (ui->recolorCheckBox->isChecked())
    {
        int index = ui->colorModeComboBox->currentIndex();
        switch (index) {
        case 0:
        {
            HillshadeWidget* w = qobject_cast<HillshadeWidget*>(ui->recolorScrollArea->widget());
            if (w != nullptr)
            {
                QString alg(" -z %1 -s %2 -az %3 -alt %4 -alg %5 %6 %7 -b %8 %9");
                params = alg.arg(QString::number(w->getVerticalExaggeration(), 'f', 2),
                                 QString::number(w->getHillshadeElevationScalar(), 'f', 2),
                                 QString::number(w->getLightAzimuth(), 'f', 2),
                                 QString::number(w->getLightAltitude(), 'f', 2),
                                 w->getAlgorithm(),
                                 w->useMultidirectionalShading() ? "-multidirectional" : "",
                                 w->useCombinedShading() ? "-combined" : "",
                                 QString::number(ui->bandSpinBox->value()),
                                 ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
            }
        } break;

        case 1:
        {
            SlopeWidget* w = qobject_cast<SlopeWidget*>(ui->recolorScrollArea->widget());
            if (w != nullptr)
            {
                QString alg(" %1 -s %2 -alg %3 -b %4 %5");
                params = alg.arg(w->usePercentValues() ? "-p" : "",
                                 QString::number(w->getSlopeElevationScalar(), 'f', 2),
                                 w->getAlgorithm(),
                                 QString::number(ui->bandSpinBox->value()),
                                 ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
            }
        } break;

        case 2:
        {
            AspectWidget* w = qobject_cast<AspectWidget*>(ui->recolorScrollArea->widget());
            if (w != nullptr)
            {
                QString alg(" %1 %2 -alg %3 -b %4 %5");
                params = alg.arg(w->useAzimuthReferredValues() ? "" : "-trigonometric",
                                 w->useZeroForFlatAreas() ? "-zero_for_flat" : "",
                                 w->getAlgorithm(),
                                 QString::number(ui->bandSpinBox->value()),
                                 ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
            }
        } break;

        case 3:
        {
            ColorReliefWidget* w = qobject_cast<ColorReliefWidget*>(ui->recolorScrollArea->widget());
            if (w != nullptr)
            {
                QString alg(" %1 -b %2 %3");
                params = alg.arg(w->useTransparency() ? "-alpha" : "",
                                 QString::number(ui->bandSpinBox->value()),
                                 ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
            }
        } break;

        default:
            break;
        }
    }

    return params;
}

/*!
 * \brief HeightMapWidget::getTRIProcessingOptionString
 * \return
 */
QString HeightMapWidget::getTRIProcessingOptionString() const
{
    QString params;

    if (ui->mapsCheckBox->isChecked() && ui->triCheckBox->isChecked())
    {
        QString alg(" -b %1 %2");
        params = alg.arg(QString::number(ui->bandSpinBox->value()),
                         ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
    }

    return params;
}

/*!
 * \brief HeightMapWidget::getTPIProcessingOptionString
 * \return
 */
QString HeightMapWidget::getTPIProcessingOptionString() const
{
    QString params;

    if (ui->mapsCheckBox->isChecked() && ui->tpiCheckBox->isChecked())
    {
        QString alg(" -b %1 %2");
        params = alg.arg(QString::number(ui->bandSpinBox->value()),
                         ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
    }

    return params;
}

/*!
 * \brief HeightMapWidget::getRoughnessProcessingOptionString
 * \return
 */
QString HeightMapWidget::getRoughnessProcessingOptionString() const
{

    QString params;

    if (ui->mapsCheckBox->isChecked() && ui->roughCheckBox->isChecked())
    {
        QString alg(" -b %1 %2");
        params = alg.arg(QString::number(ui->bandSpinBox->value()),
                         ui->edgesCheckBox->isChecked() ? "-compute_edges" : "");
    }

    return params;
}

/*!
 * \brief HeightMapWidget::getLevelColorMap
 * \return
 */
QMap<double, QColor> HeightMapWidget::getLevelColorMap()
{
    QMap<double, QColor> map;
    ColorReliefWidget* w = qobject_cast<ColorReliefWidget*>(ui->recolorScrollArea->widget());
    if (w != nullptr)
    {
        map = w->getLevelsMap();
    }
    return map;
}

/*!
 * \brief HeightMapWidget::on_colorModeComboBox_currentIndexChanged
 * \param index
 */
void HeightMapWidget::on_colorModeComboBox_currentIndexChanged(int index)
{
    QWidget* old = ui->recolorScrollArea->takeWidget();
    if (old)
    {
        old->hide();
        old->deleteLater();
    }

    switch (index) {
    case 0:
        ui->recolorScrollArea->setWidget(new HillshadeWidget());
        break;
    case 1:
        ui->recolorScrollArea->setWidget(new SlopeWidget());
        break;
    case 2:
        ui->recolorScrollArea->setWidget(new AspectWidget());
        break;
    case 3:
        ui->recolorScrollArea->setWidget(new ColorReliefWidget());
        break;
    default:
        break;
    }
}

/*!
 * \brief HeightMapWidget::on_applyButton_clicked
 */
void HeightMapWidget::on_applyButton_clicked()
{
    _outFolder = "";
    _outFolder = QFileDialog::getExistingDirectory(this, tr("Choose destination folder"),
                                                QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if (!_outFolder.isNull())
    {
        if (!_outFolder.isEmpty())
        {
            emit applyRequested();
        }
        else
        {
            QMessageBox::information(this, tr("Information"), tr("You must select a destination folder."));
        }
    }
}

/*!
 * \brief HeightMapWidget::getOutFolder
 * \return
 */
QString HeightMapWidget::getOutFolder() const
{
    return _outFolder;
}
