/****************************************************************************
**
** File: colorreliefwidget.cpp.
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
#include "colorreliefwidget.h"
#include "ui_colorreliefwidget.h"
#include "../rasterixplugin/utils/doublespinboxdelegate.h"
#include "../rasterixplugin/utils/colorpickerdelegate.h"
#include <random>

/*!
 * \brief ColorReliefWidget::ColorReliefWidget
 * \param parent
 */
ColorReliefWidget::ColorReliefWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorReliefWidget)
{
    ui->setupUi(this);

    _model = new QStandardItemModel(this);
    _model->setColumnCount(2);
    _model->setHorizontalHeaderLabels(QStringList() << tr("Levels") << tr("Color"));
    ui->reliefTableView->setModel(_model);
    ui->reliefTableView->setItemDelegateForColumn(0, new Rasterix::DoubleSpinBoxDelegate(this));
    ui->reliefTableView->setItemDelegateForColumn(1, new Rasterix::ColorPickerDelegate(this));
    ui->reliefTableView->setColumnWidth(1, 30);
}

/*!
 * \brief ColorReliefWidget::~ColorReliefWidget
 */
ColorReliefWidget::~ColorReliefWidget()
{
    delete ui;
}

/*!
 * \brief ColorReliefWidget::useTransparency
 * \return
 */
bool ColorReliefWidget::useTransparency() const
{
    return ui->alphaCheckBox->isChecked();
}

/*!
 * \brief ColorReliefWidget::getLevelsMap
 * \return
 */
QMap<double, QColor> ColorReliefWidget::getLevelsMap() const
{
    QMap<double, QColor> map;

    if (_model)
    {
        for (int i=0; i<_model->rowCount(); i++)
        {
            double h = 0.0;
            QColor c;

            h = _model->data(_model->index(i, 0), Qt::EditRole).toDouble();
            c = _model->data(_model->index(i, 1), Qt::BackgroundRole).value<QColor>();

            map.insert(h, c);
        }
    }

    return map;
}

/*!
 * \brief ColorReliefWidget::on_addButton_clicked
 */
void ColorReliefWidget::on_addButton_clicked()
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
        QModelIndex sbindex = _model->index(_model->rowCount() - 1, 0, QModelIndex());
        _model->setData(sbindex, QVariant(newLevel), Qt::EditRole);
        QModelIndex cindex = _model->index(_model->rowCount() - 1, 1, QModelIndex());
        _model->setData(cindex, QVariant(nextRandomColor()), Qt::BackgroundRole);
        _model->itemFromIndex(cindex)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
    }
}

/*!
 * \brief ColorReliefWidget::on_delButton_clicked
 */
void ColorReliefWidget::on_delButton_clicked()
{
    QItemSelectionModel* selMod = ui->reliefTableView->selectionModel();

    if (selMod != nullptr)
    {
        QModelIndexList selected = selMod->selectedRows(); // should always be a single row.
        for (auto item : selected)
            _model->removeRow(item.row());
    }
}

/*!
 * \brief ColorReliefWidget::on_clearButton_clicked
 */
void ColorReliefWidget::on_clearButton_clicked()
{
    _model->clear();
    _model->setColumnCount(2);
    _model->setHorizontalHeaderLabels(QStringList() << tr("Levels") << tr("Color"));
}

/*!
 * \brief ColorReliefWidget::nextRandomColor
 * \param prev
 * \param offset
 * \return
 */
QColor ColorReliefWidget::nextRandomColor() const
{
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_real_distribution<double> dist(0.0f, 1.0f);

    double h = dist(mt) + 0.618033988749895;
    if (h > 1) h = h - 1;
    return QColor::fromHsvF(h, 0.8f, 0.95f);
}

