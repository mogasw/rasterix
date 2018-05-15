/****************************************************************************
**
** File: gdaldriverhelper.h.
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
#ifndef GDAL_DRIVER_HELPER_H
#define GDAL_DRIVER_HELPER_H

#include "gdal_priv.h"
#include "ogrsf_frmts.h"

#include <QString>
#include <QList>

namespace Rasterix {

/*!
 * \brief The GDALDriverHelper class
 */
class GDALDriverHelper
{
public:
    static QStringList getRasterReadExtensions();
    static QStringList getRasterWriteExtensions();
    static QStringList getVectorReadExtensions();
    static QStringList getVectorWriteExtensions();
    static GDALDriver *getDriverByID(int id);
    static GDALDriver *getDriverByShortName(QString name);
    static GDALDriver *getDriverByLongName(QString name);
    static QString getLongName(GDALDriver* driver);

private:
    /*!
     * \brief The DriverType enum
     */
    enum class DriverType : int
    {
        Raster,
        Vector
    };

    /*!
     * \brief The DriverType enum
     */
    enum class DriverCapabilities : int
    {
        InOut = 0,
        In = 1,
        Out = 2
    };

    static QStringList getDriverExtensions(DriverType type, DriverCapabilities cap);
};

} // namespace Rasterix

#endif // GDAL_DRIVER_HELPER_H
