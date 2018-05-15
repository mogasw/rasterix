/****************************************************************************
**
** File: contourwidget.cpp.
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
#include "contourwidget.h"
#include "ui_contourwidget.h"
#include "../rasterixplugin/utils/doublespinboxdelegate.h"
#include "../reacticons/reacticons.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QMessageBox>

/*!
 * \brief ContourWidget::ContourWidget
 * \param parent
 */
ContourWidget::ContourWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContourWidget)
{
    ui->setupUi(this);

    _model = new QStandardItemModel(this);
    _model->setColumnCount(1);
    ui->levelsListView->setModel(_model);
    ui->levelsListView->setItemDelegate(new Rasterix::DoubleSpinBoxDelegate(this));
    ui->applyButton->setIcon(Reacticons::getIcon(u8"\uf00c", "FontAwesome", QColor(Qt::green)));
}

/*!
 * \brief ContourWidget::~ContourWidget
 */
ContourWidget::~ContourWidget()
{
    delete ui;
}

/*!
 * \brief ContourWidget::init
 * \param nBands
 * \param nodata
 */
void ContourWidget::init(int nBands, double nodata)
{
    ui->bandSpinBox->blockSignals(true);
    ui->bandSpinBox->setMinimum(1);
    ui->bandSpinBox->setMaximum(nBands);
    ui->bandSpinBox->setValue(1);
    ui->bandSpinBox->blockSignals(false);

    ui->ignoreNodataCheckBox->setEnabled((nodata != -1.0));
}

/*!
 * \brief ContourWidget::getElevationBand
 * \return
 */
int ContourWidget::getElevationBand() const
{
    return ui->bandSpinBox->value();
}

/*!
 * \brief ContourWidget::getElevationAttributeName
 * \return
 */
QString ContourWidget::getElevationAttributeName() const
{
    QString name;

    if (ui->elevCheckBox->isChecked())
        name = ui->elevLineEdit->text().simplified().trimmed();

    return name;
}

/*!
 * \brief ContourWidget::force3DVertex
 * \return
 */
bool ContourWidget::force3DVertex()
{
    return ui->treeDCheckBox->isChecked();
}

/*!
 * \brief ContourWidget::ingnoreNodata
 * \return
 */
bool ContourWidget::ingnoreNodata()
{
    return ui->ignoreNodataCheckBox->isChecked();
}

/*!
 * \brief ContourWidget::getSkipValue
 * \return
 */
double ContourWidget::getSkipValue()
{
    double result = -1.0;

    if (ui->skipCheckBox->isChecked())
        result = ui->skipSpinBox->value();

    return result;
}

/*!
 * \brief ContourWidget::getRegularInterval
 * \return
 */
double ContourWidget::getRegularInterval() const
{
    double reg = -1.0;

    if (ui->regularRadioButton->isChecked())
        reg = ui->regularIntervalSpinBox->value();

    return reg;
}

/*!
 * \brief ContourWidget::getCustomIntervals
 * \return
 */
QVector<double> ContourWidget::getCustomIntervals() const
{
    QList<double> list;

    if (_model)
    {
        for (int i=0; i<_model->rowCount(); i++)
        {
            double h = 0.0;
            h = _model->data(_model->index(i, 0), Qt::EditRole).toDouble();
            list.append(h);
        }
    }

    return list.toVector();
}

/*!
 * \brief ContourWidget::on_addLevButton_clicked
 */
void ContourWidget::on_addLevButton_clicked()
{
    if (_model != nullptr)
    {
        double newLevel = 0.0;
        if (_model->rowCount() > 0)
        {
            QModelIndex last = _model->index(_model->rowCount() - 1, 0, QModelIndex());
            newLevel = _model->data(last, Qt::EditRole).toDouble() + 20.0f;
        }

        // insert new level
        _model->setRowCount(_model->rowCount() + 1);
        QModelIndex index = _model->index(_model->rowCount() - 1, 0, QModelIndex());

        _model->setData(index, QVariant(newLevel), Qt::EditRole);

        _model->sort(1, Qt::AscendingOrder);
    }
}

/*!
 * \brief ContourWidget::on_delLevButton_clicked
 */
void ContourWidget::on_delLevButton_clicked()
{
    QItemSelectionModel* selMod = ui->levelsListView->selectionModel();

    if (selMod != nullptr)
    {
        QModelIndexList selected = selMod->selectedIndexes(); // should always be a single item.
        for (auto item : selected)
            _model->removeRow(item.row());
    }
}

/*!
 * \brief ContourWidget::on_clearLevButton_clicked
 */
void ContourWidget::on_clearLevButton_clicked()
{
    _model->clear();
    _model->setColumnCount(1);
}

/*!
 * \brief ContourWidget::getOutFolder
 * \return
 */
QString ContourWidget::getOutFolder() const
{
    return _outFolder;
}

/*!
 * \brief ContourWidget::on_applyButton_clicked
 */
void ContourWidget::on_applyButton_clicked()
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

