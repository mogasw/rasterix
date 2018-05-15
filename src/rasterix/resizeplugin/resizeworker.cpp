/****************************************************************************
**
** File: resizeworker.cpp.
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
#include "resizeworker.h"
#include "gdal_utils.h"
#include "gdal.h"

/*!
 * \brief ResizeWorker::ResizeWorker
 * \param parent
 */
ResizeWorker::ResizeWorker(QObject *parent) :
    Rasterix::Worker(parent)
{
    _mode = ResizeMode::ChangeSize;
    _resamplingAlgorithm = GRIORA_NearestNeighbour;
    _wePixelSizeMeters = 1;
    _nsPixelSizeMeters = 1;
}

/*!
 * \brief ResizeWorker::run
 */
void ResizeWorker::run()
{
    // reset last name
    _lastDatasetName = "";

    emit started();

    if (_inDataset != nullptr)
    {
        // scale to new size or change resolution
        GDALDriver *driver = _inDataset->GetDriver();
        if (driver != nullptr)
        {
            int inWidth = _inDataset->GetRasterXSize();
            int inHeight = _inDataset->GetRasterYSize();
            GDALDataType type = _inDataset->GetRasterBand(1)->GetRasterDataType();

            double destGeoTransform[6];
            _inDataset->GetGeoTransform(destGeoTransform);

            if (_mode == ResizeMode::ChangeResolution)
            {
                _outputSize.setWidth(inWidth * destGeoTransform[1] / _wePixelSizeMeters);
                _outputSize.setHeight(inHeight * destGeoTransform[5] / _nsPixelSizeMeters);

                destGeoTransform[1] = _wePixelSizeMeters;
                destGeoTransform[5] = _nsPixelSizeMeters;
            }

            int nBands = _inDataset->GetRasterCount();
            int *bandMap = new int[nBands];
            for (int i=0; i<nBands; i++)
                bandMap[i] = i+1;

            setWorkingSteps(nBands);

            // create a new temporary file
            _lastDatasetName = Worker::makeTemporaryDatasetPath(_tempDirectoryPath,
                                                                QString(driver->GetMetadataItem(GDAL_DMD_EXTENSION)));

            _outDataset = driver->Create(_lastDatasetName.toStdString().data(),
                                         _outputSize.width(), _outputSize.height(), nBands,
                                         type, nullptr);
            if (_outDataset != nullptr)
            {
                _outDataset->SetProjection(_inDataset->GetProjectionRef());
                _outDataset->SetGeoTransform(destGeoTransform);

                for(int b = 1; b <= nBands; b++)
                {
                    GDALRasterBand *inband = _inDataset->GetRasterBand(b);
                    GDALRasterBand *outband = _outDataset->GetRasterBand(b);
                    if(outband != nullptr)
                    {
                        GDALDataType type = inband->GetRasterDataType();

                        int typeSizeInBytes = GDALGetDataTypeSizeBytes(type);
                        unsigned char* pData = (unsigned char *) CPLMalloc(typeSizeInBytes * _outputSize.width() * _outputSize.height() * nBands);

                        GDALRasterIOExtraArg extraArgIn;
                        INIT_RASTERIO_EXTRA_ARG(extraArgIn);
                        extraArgIn.eResampleAlg = _resamplingAlgorithm;
                        extraArgIn.pfnProgress = (GDALProgressFunc)(&Worker::progressCallback);
                        extraArgIn.pProgressData = (void*)this;

                        // read source band
                        CPLErr errorIn = inband->RasterIO(GF_Read, 0, 0, inband->GetXSize(), inband->GetYSize(),
                                                      pData, _outputSize.width(), _outputSize.height(), type, 0, 0, &extraArgIn);

                        emit progress(0);

                        if(errorIn == CE_None)
                        {
                            GDALRasterIOExtraArg extraArgOut;
                            INIT_RASTERIO_EXTRA_ARG(extraArgOut);
                            extraArgOut.eResampleAlg = GRIORA_NearestNeighbour;
                            extraArgOut.pfnProgress = (GDALProgressFunc)(&Worker::progressCallback);
                            extraArgOut.pProgressData = (void*)this;

                            emit jobDescriptionUpdated(tr("Resizing band %1 of %2").arg(QString::number(b), QString::number(nBands)));

                            // write resized band
                            CPLErr errorOut = outband->RasterIO(GF_Write, 0, 0, _outputSize.width(), _outputSize.height(),
                                                          pData, _outputSize.width(), _outputSize.height(), type, 0, 0, &extraArgOut);

                            // copy color interpretation
                            outband->SetColorInterpretation(inband->GetColorInterpretation());

                            _currentStep++;

                            if(errorOut != CE_None)
                            {
                                emit error(tr("Error: RasterIO write operation failed."));
                            }
                        }
                        else
                        {
                            emit error(tr("Error: RasterIO read operation failed."));
                        }
                    }
                }
            }

            delete[] bandMap;
        }
    }

    emit progress(100);
    emit completed();

    setWorkingSteps(1);
}

/*!
 * \brief ResizeWorker::setResizeMode
 * \param mode
 */
void ResizeWorker::setResizeMode(ResizeMode mode)
{
    _mode = mode;
}

/*!
 * \brief ResizeWorker::setOutputSize
 * \param outputSize
 */
void ResizeWorker::setOutputSize(const QSize &outputSize)
{
    _outputSize = outputSize;
}

/*!
 * \brief ResizeWorker::setOutputResolution
 * \param weRes
 * \param nsRes
 */
void ResizeWorker::setOutputResolution(double weRes, double nsRes)
{
    _wePixelSizeMeters = weRes;
    _nsPixelSizeMeters = nsRes;

    if (_nsPixelSizeMeters > 0)
        _nsPixelSizeMeters *= -1.0;
}

/*!
 * \brief ResizeWorker::setResamplingAlgorithm
 * \param resamplingAlgorithm
 */
void ResizeWorker::setResamplingAlgorithm(int resamplingAlgorithm)
{
    _resamplingAlgorithm = (GDALRIOResampleAlg) resamplingAlgorithm;
}
