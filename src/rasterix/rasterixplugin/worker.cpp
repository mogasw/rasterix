/****************************************************************************
**
** File: worker.cpp.
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
#include "worker.h"
#include <QUuid>
#include <QDebug>

namespace Rasterix {

/*!
 * \brief Worker::Worker
 * \param parent
 */
Worker::Worker(QObject* parent) :
   QObject(parent),
   QRunnable()
{
    setAutoDelete(false);
    _inDataset = nullptr;
    _outDataset = nullptr;
    _workingSteps = 1;
    _currentStep = 0;

    // install custom error handler
    connect(this, &Worker::started, [this]{
        CPLSetErrorHandlerEx((CPLErrorHandler)(&Worker::errorCallback), (void*) this);
    });

    connect(this, &Worker::completed, []{
         CPLSetErrorHandlerEx(nullptr, nullptr);
    });
}

/*!
 * \brief Worker::setRaster
 * \param dataset
 */
void Worker::setRaster(GDALDataset* dataset)
{
    _inDataset = dataset;
}

/*!
 * \brief Worker::getProcessedRaster
 * \return
 */
GDALDataset* Worker::getProcessedRaster()
{
    return _outDataset;
}

/*!
 * \brief Worker::getProcessedDatasetName
 * \return
 */
QString Worker::getProcessedDatasetName()
{
    return _lastDatasetName;
}

/*!
 * \brief Worker::getTempDirectoryPath
 * \return
 */
QString Worker::getTempDirectoryPath() const
{
    return _tempDirectoryPath;
}

/*!
 * \brief Worker::setTempDirectoryPath
 * \param tempDirectoryPath
 */
void Worker::setTempDirectoryPath(const QString &tempDirectoryPath)
{
    _tempDirectoryPath = tempDirectoryPath;
}

/*!
 * \brief Worker::setWorkingSteps
 * \param workingSteps
 */
void Worker::setWorkingSteps(int workingSteps)
{
    _workingSteps = workingSteps;
}

/*!
 * \brief Worker::getWorkingSteps
 * \return
 */
int Worker::getWorkingSteps() const
{
    return _workingSteps;
}

/*!
 * \brief Worker::getCurrentStep
 * \return
 */
int Worker::getCurrentStep() const
{
    return _currentStep;
}

/*!
 * \brief Worker::incrementCurrentStep
 */
void Worker::incrementCurrentStep()
{
    _currentStep++;
}

/*!
 * \brief Worker::setCurrentStep
 * \param currentStep
 */
void Worker::setCurrentStep(int currentStep)
{
    _currentStep = currentStep;
}

/*!
 * \brief Worker::progressCallback
 * \param dfComplete
 * \param pszMessage
 * \param pProgressArg
 * \return
 */
int Worker::progressCallback(double dfComplete, const char *pszMessage, void *pProgressArg)
{
    Q_UNUSED(pszMessage)

    if(pProgressArg != nullptr)
    {
        Worker *self = reinterpret_cast<Worker*>(pProgressArg);
        if(self != nullptr)
        {
            float progFraction =  100.0 / self->getWorkingSteps();
            int p = (int)(self->getCurrentStep() * progFraction + dfComplete * progFraction);
            emit self->progress(p);
        }
    }

    // return true otherwise the process is interrupted
    return true;
}

/*!
 * \brief Worker::errorCallback
 * \param eErrClass
 * \param err_no
 * \param msg
 */
void Worker::errorCallback(CPLErr eErrClass, int err_no, const char *msg)
{
    Q_UNUSED(eErrClass)
    Q_UNUSED(err_no)

    Worker *self = reinterpret_cast<Worker*>(CPLGetErrorHandlerUserData());

    if (self != nullptr)
    {
        QString errorMsg = tr("GDAL error code: %1").arg(QString::number((int)eErrClass));

        if (msg != nullptr)
        {
            errorMsg.append(tr("\nMessage:\n%1").arg(QString::fromLocal8Bit(msg)));
        }

        emit self->error(errorMsg);

#ifdef QT_DEBUG
      qDebug() << errorMsg;
#endif
    }
}

/*!
 * \brief Worker::makeTemporaryDatasetName
 * \param extension
 * \return
 */
QString Worker::makeTemporaryDatasetPath(QString tempFolder, QString extension)
{
    QString uniqueName = QUuid::createUuid().toString().remove("{").remove("}");
    if (!extension.isEmpty())
        return QString("%1//%2.%3").arg(tempFolder, uniqueName, extension);
    else
        return QString("%1//%2").arg(tempFolder, uniqueName);
}

} // namespace Rasterix
