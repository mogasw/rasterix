/****************************************************************************
**
** File: gdaldriverhelper.cpp.
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
#include "gdaldriverhelper.h"
#include <cstring>

namespace Rasterix {

/*!
 * \brief GDALDriverHelper::getRasterReadExtensions
 * \return
 */
QStringList GDALDriverHelper::getRasterReadExtensions()
{
    return getDriverExtensions(DriverType::Raster, DriverCapabilities::In);
}

/*!
 * \brief GDALDriverHelper::getRasterWriteExtensions
 * \return
 */
QStringList GDALDriverHelper::getRasterWriteExtensions()
{
    return getDriverExtensions(DriverType::Raster, DriverCapabilities::Out);
}

/*!
 * \brief GDALDriverHelper::getVectorReadExtensions
 * \return
 */
QStringList GDALDriverHelper::getVectorReadExtensions()
{
    return getDriverExtensions(DriverType::Vector, DriverCapabilities::In);
}

/*!
 * \brief GDALDriverHelper::getVectorWriteExtensions
 * \return
 */
QStringList GDALDriverHelper::getVectorWriteExtensions()
{
    return getDriverExtensions(DriverType::Vector, DriverCapabilities::Out);
}

/*!
 * \brief GDALDriverHelper::getRasterExtensions
 * \param type
 * \return
 */
QStringList GDALDriverHelper::getDriverExtensions(DriverType type, DriverCapabilities cap)
{
    QStringList extensions;

    int count = GetGDALDriverManager()->GetDriverCount();

    const char* gdaltype = GDAL_DCAP_RASTER;
    switch (type)
    {
        case DriverType::Raster:
        {
            gdaltype = GDAL_DCAP_RASTER;
        }   break;

        case DriverType::Vector:
        {
            gdaltype = GDAL_DCAP_VECTOR;
        }   break;
    }

    for(int k=0; k < count; k++)
    {
        GDALDriver *driver = GetGDALDriverManager()->GetDriver(k);
        if(driver)
        {
            if(std::strncmp(driver->GetDescription(), "Memory", 32) != 0)
            {
                if(driver->GetMetadataItem(gdaltype) != 0)
                {
                    bool canRead = driver->GetMetadataItem(GDAL_DCAP_OPEN) != 0;
                    bool canWrite = (driver->GetMetadataItem(GDAL_DCAP_CREATE) != 0) &&
                                    QString(driver->GetMetadataItem(GDAL_DMD_CREATIONDATATYPES)).startsWith("Byte");

                    QString entry = QString("%1 (*.%2)").arg(QString(driver->GetMetadataItem(GDAL_DMD_LONGNAME)),
                                                             QString(driver->GetMetadataItem(GDAL_DMD_EXTENSION)));

                    switch (cap)
                    {
                        case DriverCapabilities::InOut:
                        {
                            if (canRead && canWrite)
                                extensions.append(entry);
                        }   break;

                        case DriverCapabilities::In:
                        {
                            if (canRead)
                                extensions.append(entry);
                        }   break;

                        case DriverCapabilities::Out:
                        {
                            if (canWrite)
                                extensions.append(entry);
                        }   break;
                    }
                }
            }
        }
    }

    return extensions;
}


/*!
 * \brief GDALDriverHelper::getDriverByID
 * \param id
 * \return
 */
GDALDriver *GDALDriverHelper::getDriverByID(int id)
{
    return GetGDALDriverManager()->GetDriver(id);
}

/*!
 * \brief GDALDriverHelper::getDriverByName
 * \param name
 * \return
 */
GDALDriver *GDALDriverHelper::getDriverByShortName(QString name)
{
    return GetGDALDriverManager()->GetDriverByName(name.toStdString().data());
}

/*!
 * \brief GDALDriverHelper::getDriverByLongName
 * \param name
 * \return
 */
GDALDriver *GDALDriverHelper::getDriverByLongName(QString name)
{
    GDALDriver* result = nullptr;

    int count = GetGDALDriverManager()->GetDriverCount();
    for(int k=0; k < count; k++)
    {
        GDALDriver *driver = GetGDALDriverManager()->GetDriver(k);
        if(driver)
        {
            if (QString::compare(name, QString(driver->GetMetadataItem(GDAL_DMD_LONGNAME)), Qt::CaseInsensitive) == 0)
            {
                result = driver;
                break;
            }
        }
    }

    return result;
}

/*!
 * \brief GDALDriverHelper::getLongName
 * \param driver
 * \return
 */
QString GDALDriverHelper::getLongName(GDALDriver *driver)
{
    QString name;
    if (driver)
       name =  QString(driver->GetMetadataItem(GDAL_DMD_LONGNAME));
    return name;
}

} // namespace Rasterix
