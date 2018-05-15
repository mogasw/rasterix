/****************************************************************************
**
** File: aboutpluginsdialog.cpp.
** Created: 5/2018
**
**
** Copyright (C) 2018 Gabriella Giordano
** Copyright (C) 2018 Sergio Monteleone
** Copyright (C) 2018 Moga Software s.r.l.
** All rights reserved.
**
**
** This file is part of libreacticons.
** libreacticons is a Qt wrapper for react-native-vector-icons.
**
** libreacticons is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** libreacticons is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with libreacticons. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef REACTICONS_H
#define REACTICONS_H

#include <QFontDatabase>
#include <QIcon>

/*!
 * \brief The Reacticons class
 */
class Reacticons
{
public:

    static void init();
    static QIcon getIcon(QString icon = u8"\uf07c", QString family = "FontAwesome", QColor color = QColor(0,0,0,0), int width = 64, int margin = 8);
};

#endif // REACTICONS_H
