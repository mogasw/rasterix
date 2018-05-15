/****************************************************************************
**
** File: doublespinboxdelegate.cpp.
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
#include "doublespinboxdelegate.h"
#include <QDoubleSpinBox>

namespace Rasterix {

/*!
 * \brief DoubleSpinBoxDelegate::DSpinBoxDelegate
 * \param parent
 */
DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

/*!
 * \brief DoubleSpinBoxDelegate::createEditor
 * \param parent
 * \return
 */
QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(-10000);
    editor->setMaximum(10000);
    editor->setSuffix(" m");
    editor->setDecimals(2);

    return editor;
}

/*!
 * \brief DoubleSpinBoxDelegate::setEditorData
 * \param editor
 * \param index
 */
void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

/*!
 * \brief DoubleSpinBoxDelegate::setModelData
 * \param editor
 * \param model
 * \param index
 */
void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

/*!
 * \brief DoubleSpinBoxDelegate::updateEditorGeometry
 * \param editor
 * \param option
 */
void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

/*!
 * \brief DoubleSpinBoxDelegate::displayText
 * \param value
 * \param locale
 * \return
 */
QString DoubleSpinBoxDelegate::displayText(const QVariant &value, const QLocale &/*locale*/) const
{
    return QString("%1 m").arg(QString::number(value.toDouble(), 'f', 2));
}

} // namespace Rasterix
