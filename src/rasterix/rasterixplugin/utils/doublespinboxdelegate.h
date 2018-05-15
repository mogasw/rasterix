/****************************************************************************
**
** File: doublespinboxdelegate.h.
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
#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include <QStyledItemDelegate>

namespace Rasterix {

/*!
 * \brief The DoubleSpinBoxDelegate class
 */
class DoubleSpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DoubleSpinBoxDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString displayText(const QVariant & value, const QLocale & /*locale*/) const override;
};

} // namespace Rasterix

#endif // DOUBLESPINBOXDELEGATE_H
