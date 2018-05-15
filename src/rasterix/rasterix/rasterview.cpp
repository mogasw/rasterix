/****************************************************************************
**
** File: rasterview.cpp.
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
#include "rasterview.h"

#include <QPainter>
#include <QBrush>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTimer>
#include <QDebug>

#include "gdal.h"
#include "gdal_priv.h"

namespace Rasterix {

/*!
 * \brief RasterView::RasterView
 * \param parent
 */
RasterView::RasterView(QWidget *parent) : QAbstractScrollArea(parent)
{
    dataset = nullptr;
    gridBackground = QImage(":/grid_background.png");
    backBufferFormat = QImage::Format_ARGB32;
    viewRect = QRectF(0,0,1,1);
    zoomScaleX = 1;
    zoomScaleY = 1;

    connect(this, &RasterView::completed, this, &RasterView::updateCompleted);
    connect(verticalScrollBar(), &QScrollBar::sliderMoved, this, &RasterView::verticalScrollbarMoved);
    connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this, &RasterView::horizontalScrollbarMoved);

    updateTimer = new QTimer();
    updateTimer->setInterval(500);
    connect(updateTimer, &QTimer::timeout, this, [this] {
        updateTimer->stop();
        updateBackbufferHighQuality();        
    });
}

/*!
 * \brief RasterView::getDataset
 * \return
 */
GDALDataset* RasterView::getDataset() const
{
    return dataset;
}

/*!
 * \brief RasterView::setDataset
 * \param value
 */
void RasterView::setDataset(GDALDataset* value)
{
    dataset = value;

    if(dataset != nullptr)
    {
        int nBands = dataset->GetRasterCount();
        switch(nBands)
        {
            case 1: {
                backBufferFormat = QImage::Format_Grayscale8;
            } break;
            case 3: {
                backBufferFormat = QImage::Format_RGB32;
            } break;
            case 4: {
                backBufferFormat = QImage::Format_ARGB32;
            } break;
        }

        tempBackBuffer = QImage(viewport()->size(), backBufferFormat);
        tempBackBuffer.fill(0);

        viewRect = QRectF(0, 0, dataset->GetRasterXSize(), dataset->GetRasterYSize());
        computeScalesAndAspectRatio();

        horizontalScrollBar()->setValue(viewRect.center().x());
        verticalScrollBar()->setValue(viewRect.center().y());
    }

    updateBackbuffer();
}

/*!
 * \brief RasterView::zoomIn
 */
void RasterView::zoomIn()
{
    zoom(0.7f);
}

/*!
 * \brief RasterView::zoomOut
 */
void RasterView::zoomOut()
{
    zoom(1.3f);
}

/*!
 * \brief RasterView::resizeEvent
 * \param event
 */
void RasterView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    tempBackBuffer = QImage(viewport()->size(), backBufferFormat);
    tempBackBuffer.fill(0);

    computeScalesAndAspectRatio();

    updateBackbuffer();
}

/*!
 * \brief RasterView::paintEvent
 * \param event
 */
void RasterView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(viewport());

    // grid background
    QBrush brush(gridBackground);
    painter.fillRect(rect(), brush);

    // backbuffer
    painter.drawImage(rect(), backBuffer, backBuffer.rect());
}

/*!
 * \brief RasterView::wheelEvent
 * \param event
 */
void RasterView::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
        zoomIn();
    else
        zoomOut();
}

/*!
 * \brief RasterView::mousePressEvent
 * \param event
 */
void RasterView::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    lastGeoCenter = viewRect.center();
}

/*!
 * \brief RasterView::mouseMoveEvent
 * \param event
 */
void RasterView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint deltaScreen = lastMousePosition - event->pos();

        // project delta to geo space using the correct scale factor
        QPointF deltaGeo;
        deltaGeo.setX(deltaScreen.x() / zoomScaleX);
        deltaGeo.setY(deltaScreen.y() / zoomScaleY);

        QPointF newCenter = lastGeoCenter + deltaGeo;

        viewRect.moveCenter(newCenter);
        checkViewBoundaries();

        horizontalScrollBar()->setValue(viewRect.center().x());
        verticalScrollBar()->setValue(viewRect.center().y());

        updateBackbuffer();
    }
}

/*!
 * \brief RasterView::scrollContentsBy
 * \param dx
 * \param dy
 */
void RasterView::scrollContentsBy(int /*dx*/, int /*dy*/)
{
//    viewRect.moveCenter(viewRect.center() - QPointF(dx, dy));
//    checkViewBoundaries();

//    updateBackbuffer();
}

/*!
 * \brief RasterView::zoom
 * \param scale
 */
void RasterView::zoom(float scale)
{
    QPointF center = viewRect.center();

    float w = (viewRect.width() * scale);
    float h = (viewRect.height() * scale);

    if(w <= 0) w = 2;
    if(h <= 0) h = 2;
    if(w > 100000) w = 100000;
    if(h > 100000) h = 100000;

    viewRect.setWidth(w);
    viewRect.setHeight(h);
    viewRect.moveCenter(center);

    checkViewBoundaries();

    horizontalScrollBar()->setValue(viewRect.center().x());
    verticalScrollBar()->setValue(viewRect.center().y());

    zoomScaleX = float(tempBackBuffer.width()) / viewRect.width();
    zoomScaleY = float(tempBackBuffer.height()) / viewRect.height();

    updateBackbuffer();
}

/*!
 * \brief RasterView::computeScalesAndAspectRatio
 */
void RasterView::computeScalesAndAspectRatio()
{
    float viewAR = float(height()) / float(width());
    float w = viewRect.width();
    float h = viewRect.height();
    if(width() > height())
    {
        h = w * viewAR;
        viewRect.setHeight(h);
    }
    else
    {
        w = h / viewAR;
        viewRect.setWidth(w);
    }
    checkViewBoundaries();
    zoomScaleX = float(tempBackBuffer.width()) / viewRect.width();
    zoomScaleY = float(tempBackBuffer.height()) / viewRect.height();
}

/*!
 * \brief RasterView::checkViewBoundaries
 */
void RasterView::checkViewBoundaries()
{
    float w = viewRect.width();
    float h = viewRect.height();

    if(viewRect.x() < 0)
    {
        viewRect = QRectF(0, viewRect.y(), w, h);
    }

    if(viewRect.y() < 0)
    {
        viewRect = QRectF(viewRect.x(), 0, w, h);
    }

    if(dataset != nullptr)
    {
        float viewAR = float(height()) / float(width());

        if(w > dataset->GetRasterXSize())
        {
            w = dataset->GetRasterXSize();
            h = w * viewAR;
        }

        if(h > dataset->GetRasterYSize())
        {
            h = dataset->GetRasterYSize();
            w = h / viewAR;
        }

        if(viewRect.right() >= dataset->GetRasterXSize())
        {
            viewRect = QRectF(dataset->GetRasterXSize() - w, viewRect.y(), w, h);
        }

        if(viewRect.bottom() >= dataset->GetRasterYSize())
        {
            viewRect = QRectF(viewRect.x(), dataset->GetRasterYSize() - h, w, h);
        }

        horizontalScrollBar()->setRange((viewRect.width() / 2.0), dataset->GetRasterXSize() - (viewRect.width() / 2.0));
        verticalScrollBar()->setRange((viewRect.height() / 2.0), dataset->GetRasterYSize() - (viewRect.height() / 2.0));

        horizontalScrollBar()->setPageStep(viewRect.width());
        verticalScrollBar()->setPageStep(viewRect.height());
    }    
}

/*!
 * \brief RasterView::updateBackbuffer
 */
void RasterView::updateBackbuffer()
{
    updateBackbuffer(false);
}

/*!
 * \brief RasterView::updateBackbufferHighQuality
 */
void RasterView::updateBackbufferHighQuality()
{
    updateBackbuffer(true);
}

/*!
 * \brief RasterView::updateBackbuffer
 * \param highQuality
 */
void RasterView::updateBackbuffer(bool highQuality)
{
    if(dataset == nullptr)
    {
        backBuffer.fill(0);
        return;
    }

    if(!highQuality)
        updateTimer->stop();

    // Lanczos resampling is not supported for indexed images
    if(highQuality && (backBufferFormat == QImage::Format_Grayscale8))
    {
        return;
    }

    int nBands = dataset->GetRasterCount();
    int *bandMap = new int[nBands];
    for(int b = 0; b < nBands; b++)
    {
        GDALRasterBand *band = dataset->GetRasterBand(b + 1);
        if(band != nullptr)
        {
            GDALColorInterp colorInterp = band->GetColorInterpretation();
            switch(colorInterp)
            {
                case GCI_RedBand: bandMap[b] = 3; break;
                case GCI_GreenBand: bandMap[b] = 2; break;
                case GCI_BlueBand: bandMap[b] = 1; break;
                case GCI_AlphaBand: bandMap[b] = 4; break;
                case GCI_GrayIndex: bandMap[b] = 1; break;
                case GCI_PaletteIndex : bandMap[b] = 1; break;
                default: bandMap[b] = nBands - b;
            }
        }
        else
        {
            bandMap[b] = nBands - b;
        }
    }

    int stride = tempBackBuffer.bytesPerLine();
    uchar *imageData = tempBackBuffer.scanLine(0);
    int bytesPerPixel = (backBufferFormat == QImage::Format_Grayscale8) ? 1 : 4;

    GDALRasterIOExtraArg psExtraArg;
    INIT_RASTERIO_EXTRA_ARG(psExtraArg);
    psExtraArg.bFloatingPointWindowValidity = TRUE;
    psExtraArg.dfXOff = viewRect.x();
    psExtraArg.dfYOff = viewRect.y();
    psExtraArg.dfXSize = viewRect.width();
    psExtraArg.dfYSize = viewRect.height();
    psExtraArg.eResampleAlg = (highQuality ? GRIORA_Lanczos : GRIORA_NearestNeighbour);
    psExtraArg.pfnProgress = (GDALProgressFunc)(&RasterView::progressCallback);
    psExtraArg.pProgressData = (void*)this;

    emit started();

    CPLErr error = dataset->RasterIO(GF_Read, viewRect.x(), viewRect.y(), viewRect.width(), viewRect.height(),
                                  imageData, tempBackBuffer.width(), tempBackBuffer.height(), GDT_Byte, nBands, bandMap, bytesPerPixel, stride, 1, &psExtraArg);
    if(error != CE_None)
    {
        // to do: report error
        qWarning() << "Error loading dataset for display.";
    }

    // automatically refresh the picture with high quality interpolation
    // but only if the view buffer is not too big and data is not indexed.
    if(!highQuality &&
       (viewRect.width() < 4096) &&
       (viewRect.height() < 4096) &&
       (backBufferFormat != QImage::Format_Grayscale8))
    {
        updateTimer->start();
    }

    delete[] bandMap;
}

/*!
 * \brief RasterView::udpateCompleted
 */
void RasterView::updateCompleted()
{
    backBuffer = tempBackBuffer;
    viewport()->update();
}

/*!
 * \brief RasterView::verticalScrollbarMoved
 * \param value
 */
void RasterView::verticalScrollbarMoved(int value)
{
    viewRect.moveCenter(QPointF(viewRect.center().x(), value));
    checkViewBoundaries();

    updateBackbuffer();
}

/*!
 * \brief RasterView::horizontalScrollbarMoved
 * \param value
 */
void RasterView::horizontalScrollbarMoved(int value)
{
    viewRect.moveCenter(QPointF(value, viewRect.center().y()));
    checkViewBoundaries();

    updateBackbuffer();
}

/*!
 * \brief RasterView::progressCallback
 * \param dfComplete
 * \param pszMessage
 * \param pProgressArg
 * \return
 */
int RasterView::progressCallback(double dfComplete, const char *pszMessage, void *pProgressArg)
{
    Q_UNUSED(pszMessage)

    int p = (int)(dfComplete * 100.0);

    if(pProgressArg != nullptr)
    {
        RasterView *self = reinterpret_cast<RasterView*>(pProgressArg);
        if(self != nullptr)
        {
            emit self->progress(p);

            if(p == 100)
            {
                emit self->completed();
            }
        }
    }

    // return true otherwise the process is interrupted
    return true;
}

}
