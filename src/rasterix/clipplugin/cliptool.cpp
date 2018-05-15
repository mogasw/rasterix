/****************************************************************************
**
** File: cliptool.cpp.
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
#include "cliptool.h"
#include "../reacticons/reacticons.h"

ClipTool::ClipTool(QObject *parent) :
    Rasterix::Tool(parent)
{
    _widget = new ClipWidget();
    _worker = new ClipWorker(this);
    _action = new QAction(this);
    _action->setData(QVariant::fromValue((void*) this));
    _action->setIcon(icon());
    _action->setText(name());
    _action->setToolTip(description());
    _action->setCheckable(true);
}

QString ClipTool::name()
{
    return tr("Clip");
}

QString ClipTool::description()
{
    return tr("Clip raster");
}

QIcon ClipTool::icon()
{
    return Reacticons::getIcon("", "Entypo");
}

QWidget *ClipTool::widget()
{
    return _widget;
}

Rasterix::Worker *ClipTool::worker()
{
    return _worker;
}

void ClipTool::initProcessingParams()
{
    // TODO
}

QList<QAction *> ClipTool::toolActions()
{
    return QList<QAction*>() << _action;
}

void ClipTool::initFromRaster()
{
    // TODO
}
