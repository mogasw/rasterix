/****************************************************************************
**
** File: tool.h.
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
#ifndef RASTERIX_TOOL_H
#define RASTERIX_TOOL_H

#include <QObject>
#include <QString>
#include <QWidget>
#include <QIcon>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QThreadPool>
#include <QJsonObject>
#include "worker.h"
#include "gdal.h"
#include "gdal_priv.h"

namespace Rasterix {

/*!
 * \brief The Tool class is the base class for all the tools operating
 * on raster datasets using the GDAL library.
 * Each tool must provide a name, a description, a widget for UI controls,
 * and an IRunnable istance to perform the operations on a non-GUI thread.
 */
class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(QObject* parent = nullptr);
    virtual ~Tool() {}

    virtual QString name() = 0;
    virtual QString description() = 0;
    virtual QIcon icon() = 0;
    virtual QWidget* widget() = 0;
    virtual Worker* worker() = 0;
    virtual QList<QAction*> toolActions() = 0;
    virtual QToolBar* toolBar();
    virtual void setRaster(GDALDataset* dataset);

    void setMetadata(const QJsonObject &metadata);
    QJsonObject metadata() const;

public slots:
    virtual void initProcessingParams();
    virtual void process();

protected slots:
    virtual void initFromRaster();

protected:
    QJsonObject _metadata;
    GDALDataset* _inDataset;

};

} // namespace Rasterix

#define RasterixTool_iid "org.mogasw.Qt.Rasterix.Tool"
Q_DECLARE_INTERFACE(Rasterix::Tool, RasterixTool_iid)

#endif // RASTERIX_TOOL_H
