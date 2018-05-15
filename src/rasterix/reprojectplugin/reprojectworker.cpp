/****************************************************************************
**
** File: reprojectworker.h.
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
#include "reprojectworker.h"

#include "gdalwarper.h"
#include "ogr_spatialref.h"

/*!
 * \brief ReprojectWorker::ReprojectWorker
 * \param parent
 */
ReprojectWorker::ReprojectWorker(QObject *parent) :
    Rasterix::Worker(parent)
{
    targetSRS = nullptr;
}

/*!
 * \brief ReprojectWorker::run
 */
void ReprojectWorker::run()
{
    if(targetSRS == nullptr)
        return;

    if(targetSRS->Validate() != OGRERR_NONE)
    {
        emit error(tr("Please select a valid output projection"));
        return;
    }

    // reset last name
    _lastDatasetName = "";

    emit started();

    if (_inDataset != nullptr)
    {
        // scale to new size or change resolution
        GDALDriver *driver = _inDataset->GetDriver();
        if (driver != nullptr)
        {
            {
                // setup destination coordinate system
                const char* sourceCS_WKT = _inDataset->GetProjectionRef();

                // setup destination coordinate system
                const char* destCS_WKT = nullptr;
                targetSRS->exportToWkt(const_cast<char**>(&destCS_WKT));

                // create a transformer that maps from source pixel/line coordinates
                // to destination georeferenced coordinates (not destination
                // pixel line). We do that by omitting the destination dataset
                // handle (setting it to nullptr)
                void* projTransformer = GDALCreateGenImgProjTransformer(_inDataset, sourceCS_WKT, nullptr, destCS_WKT, false, 0, 1);
                if (projTransformer != nullptr)
                {
                    double destGeoTransform[6];
                    double sourceExtent[4];
                    int pixelCount = 0;
                    int linesCount = 0;
                    CPLErr result = GDALSuggestedWarpOutput2(_inDataset, GDALGenImgProjTransform, projTransformer, destGeoTransform, &pixelCount , &linesCount, sourceExtent, 0);

                    // clean up
                    GDALDestroyGenImgProjTransformer(projTransformer);
                    if (result == CE_None)
                    {
                        int srcBandCount = _inDataset->GetRasterCount();

                        // use the source's datatype, taken from first input band
                        GDALDataType sourceDataType = _inDataset->GetRasterBand(1)->GetRasterDataType();

                        // create the output file
                        // create a new temporary file
                        _lastDatasetName = Worker::makeTemporaryDatasetPath(_tempDirectoryPath,
                                                                            QString(driver->GetMetadataItem(GDAL_DMD_EXTENSION)));

                        _outDataset = driver->Create(_lastDatasetName.toStdString().data(),
                                                     pixelCount, linesCount, srcBandCount,
                                                     sourceDataType, nullptr);

                        if (_outDataset != nullptr)
                        {
                            // write out the projection definition
                            GDALSetProjection(_outDataset, destCS_WKT);
                            GDALSetGeoTransform(_outDataset, destGeoTransform);

                            // copy the color table and no data value for each band
                            double *noDataValues = (double *) CPLMalloc(sizeof(double) * srcBandCount);
                            for (int i=0; i<srcBandCount; i++)
                            {
                                GDALRasterBandH srcBand = GDALGetRasterBand(_inDataset, i+1);
                                GDALRasterBandH dstBand = GDALGetRasterBand(_outDataset, i+1);
                                GDALColorTableH sourceColorTable;
                                sourceColorTable = GDALGetRasterColorTable(srcBand);
                                GDALSetRasterColorInterpretation(dstBand, GDALGetRasterColorInterpretation(srcBand));
                                if(sourceColorTable != nullptr)
                                {
                                    GDALSetRasterColorTable(dstBand, sourceColorTable);
                                }

                                // set no data value to zero by default
                                noDataValues[i] = 0;
                            }

                            // setup warp options
                            GDALWarpOptions *warpOptions = GDALCreateWarpOptions();
                            warpOptions->hSrcDS = _inDataset;
                            warpOptions->hDstDS = _outDataset;
                            warpOptions->nBandCount = srcBandCount;
                            warpOptions->panSrcBands = (int *) CPLMalloc(sizeof(int) * warpOptions->nBandCount);
                            for(int b=0; b <warpOptions->nBandCount; b++)
                                warpOptions->panSrcBands[b] = (b+1);
                            warpOptions->panDstBands = (int *) CPLMalloc(sizeof(int) * srcBandCount);
                            for(int b=0; b < srcBandCount; b++)
                                warpOptions->panDstBands[b] = (b+1);

                            // init no data value
                            warpOptions->padfSrcNoDataReal = (double *) CPLMalloc(sizeof(double) * warpOptions->nBandCount);
                            warpOptions->padfSrcNoDataImag = (double *) CPLMalloc(sizeof(double) * warpOptions->nBandCount);
                            for(int b=0; b <warpOptions->nBandCount; b++)
                            {
                                warpOptions->padfSrcNoDataReal[b] = noDataValues[b];
                                warpOptions->padfSrcNoDataImag[b] = noDataValues[b];
                            }

                            warpOptions->padfDstNoDataReal = (double *) CPLMalloc(sizeof(double) * srcBandCount);
                            warpOptions->padfDstNoDataImag = (double *) CPLMalloc(sizeof(double) * srcBandCount);
                            for(int b=0; b < srcBandCount; b++)
                            {
                                warpOptions->padfDstNoDataReal[b] = noDataValues[b];
                                warpOptions->padfDstNoDataImag[b] = noDataValues[b];
                            }

                            warpOptions->pfnProgress = (GDALProgressFunc)(&Worker::progressCallback);
                            warpOptions->pProgressArg = (void*)this;

                            char **options = NULL;
                            options = CSLSetNameValue(options, "NUM_THREADS", "ALL_CPUS");
                            warpOptions->papszWarpOptions = options;

                            // establish reprojection transformer
                            warpOptions->pTransformerArg = GDALCreateGenImgProjTransformer(_inDataset, sourceCS_WKT,
                                                                                           _outDataset, destCS_WKT, false, 0.0, 1);
                            warpOptions->pfnTransformer = GDALGenImgProjTransform;

                            // initialize and execute the warp operation
                            GDALWarpOperation warpOperation;
                            result = warpOperation.Initialize(warpOptions);
                            if (result == CE_None)
                            {
                                result = warpOperation.ChunkAndWarpImage(0, 0, _outDataset->GetRasterXSize(), _outDataset->GetRasterYSize());
                                if (result != CE_None)
                                {
                                    emit error(tr("Reprojection operation failed."));
                                }

                                // clean up
                                GDALDestroyGenImgProjTransformer(warpOptions->pTransformerArg);
                                GDALDestroyWarpOptions(warpOptions);
                            }
                        }
                    }
                }
            }
        }
    }

    emit progress(100);
    emit completed();
}

/*!
 * \brief ReprojectWorker::setTargetSRS
 * \param value
 */
void ReprojectWorker::setTargetSRS(OGRSpatialReference *value)
{
    targetSRS = value;
}
