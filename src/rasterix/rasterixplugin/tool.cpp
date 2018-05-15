/****************************************************************************
**
** File: tool.cpp.
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
#include "tool.h"

namespace Rasterix {

/*!
 * \brief Tool::Tool
 * \param parent
 */
Tool::Tool(QObject* parent) :
    QObject(parent)
{
    _inDataset = nullptr;
}

/*!
 * \brief toolBar
 * \return
 */
QToolBar* Tool::toolBar()
{
    return nullptr;
}

/*!
 * \brief Tool::setRaster
 * \param dataset
 */
void Tool::setRaster(GDALDataset *dataset)
{
    _inDataset = dataset;

    initFromRaster();
}

/*!
 * \brief setMetadata
 * \param metadata
 */
void Tool::setMetadata(const QJsonObject &metadata)
{
    _metadata = metadata;
}

/*!
 * \brief metadata
 * \return the plugin metadata as a QJsonObject instance
 */
QJsonObject Tool::metadata() const
{
    return _metadata;
}

/*!
 * \brief Tool::initProcessingParams
 */
void Tool::initProcessingParams()
{
    // for override
}

/*!
 * \brief Tool::process
 */
void Tool::process()
{
    initProcessingParams();

    Worker* runnable = worker();
    if (runnable != nullptr)
    {
        QThreadPool *threadPool = QThreadPool::globalInstance();
        threadPool->start(runnable);
    }
}

/*!
 * \brief Tool::initFromRaster
 */
void Tool::initFromRaster()
{
    // for override
}

}
