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
#include "reacticons.h"

#include <QPainter>
#include <QFont>
#include <QPalette>
#include <QGuiApplication>

/*!
 * \brief Reacticons::init
 */
void Reacticons::init()
{
    Q_INIT_RESOURCE(iconfonts);

    QFontDatabase::addApplicationFont(":/Zocial.ttf");
    QFontDatabase::addApplicationFont(":/SimpleLineIcons.ttf");
    QFontDatabase::addApplicationFont(":/Octicons.ttf");
    QFontDatabase::addApplicationFont(":/MaterialIcons.ttf");
    QFontDatabase::addApplicationFont(":/MaterialCommunityIcons.ttf");
    QFontDatabase::addApplicationFont(":/Ionicons.ttf");
    QFontDatabase::addApplicationFont(":/Foundation.ttf");
    QFontDatabase::addApplicationFont(":/FontAwesome.ttf");
    QFontDatabase::addApplicationFont(":/Feather.ttf");
    QFontDatabase::addApplicationFont(":/EvilIcons.ttf");
    QFontDatabase::addApplicationFont(":/Entypo.ttf");
}

/*!
 * \brief Reacticons::getIcon
 * \param family
 * \param icon
 * \return
 */
QIcon Reacticons::getIcon(QString icon, QString family, QColor color, int width, int margin)
{
    QPixmap pixmap(width, width);
    pixmap.fill(QColor(0,0,0,0));

    QPainter p(&pixmap);

    QFont font;
    font.setFamily(family);
    font.setPixelSize(pixmap.width() - 2*margin);
    p.setFont(font);

    // default color is taken from current QPalette
    if(color.alpha() == 0)
    {
        color = QGuiApplication::palette().text().color();
    }

    p.setPen(color);

    QRect rect(margin, margin, pixmap.width() - 2*margin, pixmap.height() - 2*margin);

    p.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, icon);

    return QIcon(pixmap);
}
