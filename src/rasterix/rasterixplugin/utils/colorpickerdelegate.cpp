/****************************************************************************
**
** File: colorpickerdelegate.cpp.
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
#include "colorpickerdelegate.h"
#include <QColorDialog>

namespace Rasterix {

/*!
 * \brief ColorPickerDelegate::ColorPickerDelegate
 * \param parent
 */
ColorPickerDelegate::ColorPickerDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

/*!
 * \brief ColorPickerDelegate::createEditor
 * \param parent
 * \param option
 * \param index
 * \return
 */
QWidget *ColorPickerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QColorDialog* editor = new QColorDialog(parent);

    return editor;
}

/*!
 * \brief ColorPickerDelegate::setEditorData
 * \param editor
 * \param index
 */
void ColorPickerDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QColor currentColor = index.data(Qt::BackgroundRole).value<QColor>();

    QColorDialog *dialog = static_cast<QColorDialog*>(editor);
    dialog->setCurrentColor(currentColor);
}

/*!
 * \brief ColorPickerDelegate::setModelData
 * \param editor
 * \param model
 * \param index
 */
void ColorPickerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QColorDialog *dialog = static_cast<QColorDialog*>(editor);

    QColor currentColor = dialog->currentColor();
    model->setData(index, currentColor, Qt::BackgroundRole);
}

/*!
 * \brief ColorPickerDelegate::updateEditorGeometry
 * \param editor
 * \param option
 * \param index
 */
void ColorPickerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}


} // namespace Rasterix
