/****************************************************************************
**
** File: nodataworker.cpp.
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
#include "nodataworker.h"
#include "gdal_alg.h"

/*!
 * \brief NoDataWorker::NoDataWorker
 * \param parent
 */
NoDataWorker::NoDataWorker(QObject *parent) :
    Rasterix::Worker(parent)
{
    _mode = NoDataFillMode::ByValue;
    _newNodataValue = 0;
    _maxDistancePixels = 0;
    _smoothingPasses = 1;
}

/*!
 * \brief NoDataWorker::run
 */
void NoDataWorker::run()
{
    // reset last name
    _lastDatasetName = "";

    if (_inDataset != nullptr)
    {
        GDALDriver *driver = _inDataset->GetDriver();
        if (driver != nullptr)
        {
            int nBands = _inDataset->GetRasterCount();

            _lastDatasetName = Worker::makeTemporaryDatasetPath(_tempDirectoryPath,
                                                                QString(driver->GetMetadataItem(GDAL_DMD_EXTENSION)));

            setWorkingSteps(nBands + 1 /* copy step */); // show multistep progress
            setCurrentStep(0);
            emit started();

            _outDataset = driver->CreateCopy(_lastDatasetName.toStdString().data(), _inDataset,
                                             0, nullptr,
                                             (GDALProgressFunc)(&Worker::progressCallback),
                                             (void*)this);

            if (_outDataset)
            {
                switch (_mode)
                {
                case NoDataFillMode::Smoothing:
                    {
                        // fill nodata for each band
                        for (int i = 1; i <= nBands; i++)
                        {
                            GDALRasterBand *outband = _outDataset->GetRasterBand(i);
                            if (outband)
                            {
                                GDALRasterBand *maskBand = outband->GetMaskBand();
                                if (maskBand)
                                {
                                    setCurrentStep(i);

                                    emit jobDescriptionUpdated(tr("Filling nodata values"));

                                    CPLErr err = GDALFillNodata(outband,
                                                                maskBand,
                                                                _maxDistancePixels,
                                                                0 /* deprecated option */,
                                                                _smoothingPasses,
                                                                nullptr,
                                                                (GDALProgressFunc)(&Worker::progressCallback),
                                                                (void*)this);

                                    if (err != CE_None)
                                    {
                                        emit error(tr("Error: fill nodata failed for band #%1").arg(i));
                                    }
                                }
                            }
                        }
                    }
                    break;

                case NoDataFillMode::ByValue :
                    {
                        for (int i = 1; i <= nBands; i++)
                        {
                            GDALRasterBand *outband = _outDataset->GetRasterBand(i);
                            if (outband)
                            {
                                CPLErr err = outband->SetNoDataValue(_newNodataValue);

                                if (err != CE_None)
                                {
                                    emit error(tr("Error: could not set nodata value to band #%1").arg(i));
                                }

                                emit progress(i * 100.0 / nBands);
                            }
                        }

                    }
                    break;

                default:
                        break;
                }
            }
            else
            {
                emit error(tr("Error: could not create a copy of this raster dataset"));
            }

            emit progress(100);
            emit completed();
        }
    }

    setWorkingSteps(1); // reset steps.
}

/*!
 * \brief NoDataWorker::setMode
 * \param mode
 */
void NoDataWorker::setMode(const NoDataFillMode &mode)
{
    _mode = mode;
}

/*!
 * \brief NoDataWorker::setNewNodataValue
 * \param newNodataValue
 */
void NoDataWorker::setNewNodataValue(double newNodataValue)
{
    _newNodataValue = newNodataValue;
}

/*!
 * \brief NoDataWorker::setMaxDistance
 * \param maxDistance
 */
void NoDataWorker::setMaxDistancePixels(double maxDistance)
{
    _maxDistancePixels = maxDistance;
}

/*!
 * \brief NoDataWorker::setWinLength
 * \param winLength
 */
void NoDataWorker::setSmoothingPasses(int winLength)
{
    _smoothingPasses = winLength;
}
