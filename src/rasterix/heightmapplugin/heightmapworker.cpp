/****************************************************************************
**
** File: heightmapworker.cpp.
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
#include "heightmapworker.h"
#include "gdal_utils.h"
#include "gdal_alg.h"
#include <QFile>
#include <QDebug>

/*!
 * \brief HeightMapWorker::HeightMapWorker
 * \param parent
 */
HeightMapWorker::HeightMapWorker(QObject *parent) :
    Rasterix::Worker(parent)
{

}

/*!
 * \brief HeightMapWorker::run
 */
void HeightMapWorker::run()
{
    // reset last name
    _lastDatasetName = "";

    emit started();

    if (_inDataset != nullptr)
    {
        int usageErr = 0;

        // count working steps
        int wsteps = 0;
        wsteps = (_demProcessingOptions.isEmpty()) ? wsteps : wsteps + 1;
        wsteps = (_triProcessingOptions.isEmpty()) ? wsteps : wsteps + 1;
        wsteps = (_tpiProcessingOptions.isEmpty()) ? wsteps : wsteps + 1;
        wsteps = (_roughnessProcessingOptions.isEmpty()) ? wsteps : wsteps + 1;
        setWorkingSteps(wsteps);

        if (!_demAlgorithm.isEmpty() && !_demProcessingOptions.isEmpty())
        {
            QString outDemName("%1/%2.tif");
            outDemName = outDemName.arg(_outFolderPath, _demAlgorithm);
            QString tmpColorFileName = "";

            if (_demAlgorithm.compare(QString("color-relief"), Qt::CaseInsensitive) == 0)
            {
                // write color rilief file in temp folder.
                tmpColorFileName = makeTemporaryDatasetPath(_tempDirectoryPath, "txt");

                if (!_colorReliefMap.isEmpty())
                {
                    QFile tmpFile(tmpColorFileName);
                    if (tmpFile.open(QFile::WriteOnly))
                    {
                        QList<double> levels = _colorReliefMap.keys();
                        for (auto l : levels)
                        {
                            QColor c = _colorReliefMap[l];
                            QString line("%1, %2, %3, %4, %5\n");
                            line = line.arg(QString::number(l),
                                            QString::number(c.red()),
                                            QString::number(c.green()),
                                            QString::number(c.blue()),
                                            QString::number(c.alpha()));
                            tmpFile.write(line.toStdString().data());
                        }

                        tmpFile.close();
                    }
                }
            }

            char** optList = makeOptionList(_demProcessingOptions);
            GDALDEMProcessingOptions* demOpts = GDALDEMProcessingOptionsNew(optList, nullptr);
            if (demOpts != nullptr)
            {
                GDALDEMProcessingOptionsSetProgress(demOpts,
                                                    (GDALProgressFunc) (&Worker::progressCallback),
                                                    (void*) this);

                emit jobDescriptionUpdated(tr("Heighmap generation (%1)").arg(_demAlgorithm));

                GDALDataset* out = (GDALDataset*) GDALDEMProcessing(outDemName.toStdString().data(),
                                                     _inDataset,
                                                     _demAlgorithm.toStdString().data(),
                                                     tmpColorFileName.isEmpty() ? nullptr : tmpColorFileName.toStdString().data(),
                                                     demOpts,
                                                     &usageErr);

                GDALDEMProcessingOptionsFree(demOpts);
                CSLDestroy(optList);

                if (out == nullptr || usageErr != 0)
                {
                    emit error("Error: could not create output raster.");
                }

                GDALClose(out);
            }

            _currentStep++;
        }

        if (!_triProcessingOptions.isEmpty())
        {
            QString outTRIName("%1/tri_map.tif");
            outTRIName = outTRIName.arg(_outFolderPath);
            char** optList = makeOptionList(_triProcessingOptions);

            GDALDEMProcessingOptions* triOpts = GDALDEMProcessingOptionsNew(optList, nullptr);
            GDALDEMProcessingOptionsSetProgress(triOpts,
                                                (GDALProgressFunc) (&Worker::progressCallback),
                                                (void*) this);

            emit jobDescriptionUpdated(tr("Generating TRI map"));

            GDALDataset* out = (GDALDataset*) GDALDEMProcessing(outTRIName.toStdString().data(),
                                                 _inDataset,
                                                 "TRI",
                                                 nullptr,
                                                 triOpts,
                                                 &usageErr);

            GDALDEMProcessingOptionsFree(triOpts);
            CSLDestroy(optList);

            if (out == nullptr || usageErr != 0)
            {
                emit error("Error: could not create TRI map.");
            }

            GDALClose(out);

            _currentStep++;
        }

        if (!_tpiProcessingOptions.isEmpty())
        {
            QString outTPIName("%1/tpi_map.tif");
            outTPIName = outTPIName.arg(_outFolderPath);
            char** optList = makeOptionList(_tpiProcessingOptions);

            GDALDEMProcessingOptions* tpiOpts = GDALDEMProcessingOptionsNew(optList, nullptr);
            GDALDEMProcessingOptionsSetProgress(tpiOpts,
                                                (GDALProgressFunc) (&Worker::progressCallback),
                                                (void*) this);

            emit jobDescriptionUpdated(tr("Generating TPI map"));

            GDALDataset* out = (GDALDataset*) GDALDEMProcessing(outTPIName.toStdString().data(),
                                                 _inDataset,
                                                 "TPI",
                                                 nullptr,
                                                 tpiOpts,
                                                 &usageErr);

            GDALDEMProcessingOptionsFree(tpiOpts);
            CSLDestroy(optList);

            if (out == nullptr || usageErr != 0)
            {
                emit error("Error: could not create TPI map.");
            }

            GDALClose(out);

            _currentStep++;
        }

        if (!_roughnessProcessingOptions.isEmpty())
        {
            emit jobDescriptionUpdated(tr("Generating Roughness map"));

            QString outRoughName("%1/roughness_map.tif");
            outRoughName = outRoughName.arg(_outFolderPath);
            char** optList = makeOptionList(_roughnessProcessingOptions);

            GDALDEMProcessingOptions* roughOpts = GDALDEMProcessingOptionsNew(optList, nullptr);
            GDALDEMProcessingOptionsSetProgress(roughOpts,
                                                (GDALProgressFunc) (&Worker::progressCallback),
                                                (void*) this);

            GDALDataset* out = (GDALDataset*) GDALDEMProcessing(outRoughName.toStdString().data(),
                                                 _inDataset,
                                                 "roughness",
                                                 nullptr,
                                                 roughOpts,
                                                 &usageErr);

            GDALDEMProcessingOptionsFree(roughOpts);
            CSLDestroy(optList);

            if (out == nullptr || usageErr != 0)
            {
                emit error("Error: could not create roughness map.");
            }

            GDALClose(out);

            _currentStep++;
        }

    }

    emit progress(100);
    emit completed();

    setWorkingSteps(1);
}

/*!
 * \brief HeightMapWorker::setDemAlgorithm
 * \param demAlgorithm
 */
void HeightMapWorker::setDemAlgorithm(const QString &demAlgorithm)
{
    _demAlgorithm = demAlgorithm;
}

/*!
 * \brief HeightMapWorker::setDemProcessingOptions
 * \param demProcessingOptions
 */
void HeightMapWorker::setDemProcessingOptions(const QString &demProcessingOptions)
{
    _demProcessingOptions = demProcessingOptions;
}

/*!
 * \brief HeightMapWorker::setTriProcessingOptions
 * \param triProcessingOptions
 */
void HeightMapWorker::setTriProcessingOptions(const QString &triProcessingOptions)
{
    _triProcessingOptions = triProcessingOptions;
}

/*!
 * \brief HeightMapWorker::setTpiProcessingOptions
 * \param tpiProcessingOptions
 */
void HeightMapWorker::setTpiProcessingOptions(const QString &tpiProcessingOptions)
{
    _tpiProcessingOptions = tpiProcessingOptions;
}

/*!
 * \brief HeightMapWorker::setRoughnessProcessingOptions
 * \param roughnessProcessingOptions
 */
void HeightMapWorker::setRoughnessProcessingOptions(const QString &roughnessProcessingOptions)
{
    _roughnessProcessingOptions = roughnessProcessingOptions;
}

/*!
 * \brief HeightMapWorker::setColorReliefMap
 * \param colorReliefMap
 */
void HeightMapWorker::setColorReliefMap(const QMap<double, QColor> &colorReliefMap)
{
    _colorReliefMap = colorReliefMap;
}

/*!
 * \brief HeightMapWorker::setOutFolderPath
 * \param outFolderPath
 */
void HeightMapWorker::setOutFolderPath(const QString &outFolderPath)
{
    _outFolderPath = outFolderPath;
}

/*!
 * \brief HeightMapWorker::makeOptionList
 * \param algorithm
 * \param opts
 * \return
 */
char **HeightMapWorker::makeOptionList(QString opts)
{
    char** list = nullptr;

    QStringList parts = opts.simplified().split(" ");
    for (int i=0; i<parts.length(); i++)
    {
        QString p = parts[i];
        if (!p.isEmpty())
            list = CSLAddString(list, parts[i].toStdString().data());
    }
    return list;
}
