/****************************************************************************
**
** File: worker.h.
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
#ifndef RASTERIX_WORKER_H
#define RASTERIX_WORKER_H

#include <QObject>
#include <QRunnable>
#include "gdal.h"
#include "gdal_priv.h"

namespace Rasterix {

/*!
 * \brief The Worker class is the base class used to implement
 * GDAL operations (e.g. warping, resize, contouring etc.).
 * Worker inherits from QRunnable, and therefore each inhering class
 * must override the QRunnable::run() method.
 */
class Worker :
        public QObject,
        public QRunnable
{
    Q_OBJECT
public:
    Worker(QObject* parent = 0);
    virtual ~Worker() {}

    virtual void setRaster(GDALDataset* dataset);
    virtual GDALDataset* getProcessedRaster();
    virtual QString getProcessedDatasetName();

    QString getTempDirectoryPath() const;
    void setTempDirectoryPath(const QString &tempDirectoryPath);
    void setWorkingSteps(int workingSteps);
    int getWorkingSteps() const;
    int getCurrentStep() const;
    void incrementCurrentStep();
    void setCurrentStep(int currentStep);

signals:
    void started();
    void progress(int p);
    void completed();
    void jobDescriptionUpdated(QString job);
    void error(QString message);

protected:
    GDALDataset* _inDataset;
    GDALDataset* _outDataset;
    QString _lastDatasetName;
    QString _tempDirectoryPath;
    int _workingSteps;
    int _currentStep;

    static int progressCallback(double dfComplete, const char* pszMessage, void* pProgressArg);
    static void errorCallback(CPLErr eErrClass, int err_no, const char *msg);

    static QString makeTemporaryDatasetPath(QString tempFolder, QString extension);
};

} // namespace Rasterix

#define RasterixWorker_iid "org.mogasw.Qt.Rasterix.Worker"
Q_DECLARE_INTERFACE(Rasterix::Worker, RasterixWorker_iid)

#endif // RASTERIX_WORKER_H
