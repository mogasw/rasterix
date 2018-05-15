/****************************************************************************
**
** File: contourworker.cpp.
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
#include "contourworker.h"
#include "gdaldriverhelper.h"
#include "gdal_alg.h"
#include <ogr_core.h>
#include <QVector>
#include <QFile>

/*!
 * \brief ContourWorker::ContourWorker
 * \param parent
 */
ContourWorker::ContourWorker(QObject *parent) :
    Rasterix::Worker(parent)
{
    _bandIndex = 1;
    _baseLevel = 0;
    _regularInterval = 50;
    _3D = false;
    _ignoreNodata = true;
    _skipValue = 0;
    _elevationAttrName = "ELEV";
}

/*!
 * \brief ContourWorker::run
 */
void ContourWorker::run()
{
    // reset last name
    _lastDatasetName = "";

    emit started();

    if (_inDataset != nullptr)
    {
        GDALRasterBand* band = _inDataset->GetRasterBand(_bandIndex);

        GDALDriver* shpDriver = Rasterix::GDALDriverHelper::getDriverByLongName("ESRI Shapefile");
        if (shpDriver != nullptr)
        {
            // create shape file dataset
            GDALDataset* contourDataset = (GDALDataset*) shpDriver->Create(_outFolderPath.toStdString().data(), 0, 0, 0, GDT_Unknown, nullptr);

            if (contourDataset != nullptr)
            {
                OGRSpatialReference* spatialReference = new OGRSpatialReference(_inDataset->GetProjectionRef());
                if (spatialReference != nullptr)
                {
                    OGRLayer* layer =  contourDataset->CreateLayer("contour",
                                                                   spatialReference,
                                                                   _3D ? wkbLineString25D : wkbLineString,
                                                                   nullptr);

                    if (layer)
                    {
                        OGRFieldDefn* idDefn = new OGRFieldDefn( "ID", OFTInteger );
                        idDefn->SetWidth(8);
                        layer->CreateField(idDefn, false);
                        delete idDefn;

                        if (!_elevationAttrName.isEmpty())
                        {
                            OGRFieldDefn* elevDefn = new OGRFieldDefn(_elevationAttrName.toStdString().data(), OFTReal);
                            elevDefn->SetWidth(12);
                            elevDefn->SetPrecision(3);
                            layer->CreateField(elevDefn);
                            delete elevDefn;
                        }

                        emit jobDescriptionUpdated(tr("Extract contouring"));

                        // generate contours
                        CPLErr err = GDALContourGenerate(band,
                                                         _regularInterval,
                                                         _baseLevel,
                                                         _customIntervals.length(),
                                                         _customIntervals.toStdVector().data(),
                                                         _ignoreNodata, /* ignore nodata in dataset */
                                                         _skipValue,    /* nodata value */
                                                         layer,
                                                         layer->FindFieldIndex("ID", true), /* make unique Feature IDs */
                                                         layer->FindFieldIndex(_elevationAttrName.toStdString().data(), true), /* elevation index of the dataset, or -1 if not given */
                                                         (GDALProgressFunc)(&Worker::progressCallback),
                                                         (void*)this);



                        if (err != CE_None)
                        {
                            emit error(tr("Error: contouring failed"));
                        }

                    }
                    else
                    {
                        // no layer
                        emit error(tr("Error: Could not create a layer for contouring"));
                    }

                    delete spatialReference;
                }
                else
                {
                    // no spatial reference
                    emit error(tr("Error: no valid spatial reference provided"));
                }

                // close contour dataset and flush to disk
                GDALClose(contourDataset);
            }
            else
            {
                // no vector dataset
                emit error(tr("Error: Could not create a vector dataset for contouring"));
            }
        }
    }

    emit progress(100);
    emit completed();
}

/*!
 * \brief ContourWorker::setBandIndex
 * \param bandIndex
 */
void ContourWorker::setBandIndex(int bandIndex)
{
    _bandIndex = bandIndex;
}

/*!
 * \brief ContourWorker::setBaseLevel
 * \param baseLevel
 */
void ContourWorker::setBaseLevel(double baseLevel)
{
    _baseLevel = baseLevel;
}

/*!
 * \brief ContourWorker::setRegularInterval
 * \param regularInterval
 */
void ContourWorker::setRegularInterval(double regularInterval)
{
    _regularInterval = regularInterval;
}

/*!
 * \brief ContourWorker::setCustomIntervals
 * \param customIntervals
 */
void ContourWorker::setCustomIntervals(const QVector<double> &customIntervals)
{
    _customIntervals = customIntervals;
}

/*!
 * \brief ContourWorker::setIgnoreNodata
 * \param ignoreNodata
 */
void ContourWorker::setIgnoreNodata(bool ignoreNodata)
{
    _ignoreNodata = ignoreNodata;
}

/*!
 * \brief ContourWorker::setSkipValue
 * \param skipValue
 */
void ContourWorker::setSkipValue(double skipValue)
{
    _skipValue = skipValue;
}

/*!
 * \brief ContourWorker::setElevationAttrName
 * \param elevationAttrName
 */
void ContourWorker::setElevationAttrName(const QString &elevationAttrName)
{
    _elevationAttrName = elevationAttrName;
}

/*!
 * \brief ContourWorker::set3D
 * \param threeD
 */
void ContourWorker::set3D(bool threeD)
{
    _3D = threeD;
}

void ContourWorker::setOutFolderPath(const QString &outFolderPath)
{
    _outFolderPath = outFolderPath;
}
