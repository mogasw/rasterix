/****************************************************************************
**
** File: exportworker.cpp.
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
#include "exportworker.h"
#include "../rasterixplugin/gdaldriverhelper.h"

/*!
 * \brief ExportWorker::ExportWorker
 * \param parent
 */
ExportWorker::ExportWorker(QObject *parent) :
    Rasterix::Worker(parent)
{

}

/*!
 * \brief ExportWorker::run
 */
void ExportWorker::run()
{
    emit started();

    _outDataset = nullptr;

    if (_inDataset != nullptr)
    {
        GDALDriver* driver = Rasterix::GDALDriverHelper::getDriverByLongName(_driverName);

        if (driver != nullptr)
        {
            char* opt = nullptr;

            emit jobDescriptionUpdated(tr("Saving"));

            _outDataset = driver->CreateCopy(_outputFileName.toStdString().data(), _inDataset, 0, &opt,
                                             (GDALProgressFunc)(&Worker::progressCallback),
                                             (void*) this);
        }
    }

    emit progress(100);
    emit completed();
}

/*!
 * \brief ExportWorker::setOutputFileName
 * \param outputFileName
 */
void ExportWorker::setOutputFileName(const QString &outputFileName)
{
    _outputFileName = outputFileName;
}

/*!
 * \brief ExportWorker::setDriverName
 * \param driverName
 */
void ExportWorker::setDriverName(const QString &driverName)
{
    _driverName = driverName;
}


