/****************************************************************************
**
** File: rasterview.h.
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
#ifndef RASTERVIEW_H
#define RASTERVIEW_H

#include <QAbstractScrollArea>

#include "gdal.h"
#include "gdal_priv.h"

namespace Rasterix {


/*!
 * \brief RasterView::RasterView
 * RasterView widget shows the raster dataset on screen.
 */
class RasterView : public QAbstractScrollArea
{
    Q_OBJECT
public:

    /*!
     * \brief RasterView
     * \param parent
     */
    explicit RasterView(QWidget *parent = nullptr);

    GDALDataset* getDataset() const;
    void setDataset(GDALDataset* value);

    void zoomIn();
    void zoomOut();

signals:
    /*!
     * \brief started
     */
    void started();

    /*!
     * \brief progress
     * \param p
     */
    void progress(int p);

    /*!
     * \brief completed
     */
    void completed();

    /*!
     * \brief error
     * \param message
     */
    void error(QString message);

public slots:

protected:
    GDALDataset* dataset;
    QImage backBuffer;
    QImage tempBackBuffer;
    QImage gridBackground;
    QImage::Format backBufferFormat;
    QRectF viewRect;
    QPoint lastMousePosition;
    QPointF lastGeoCenter;
    QTimer *updateTimer;
    float zoomScaleX;
    float zoomScaleY;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void scrollContentsBy(int, int) override;

    void zoom(float scale);
    void computeScalesAndAspectRatio();
    void checkViewBoundaries();

    static int progressCallback(double dfComplete, const char *pszMessage, void *pProgressArg);

protected slots:
    void updateBackbuffer();
    void updateBackbufferHighQuality();
    void updateBackbuffer(bool highQuality);
    void updateCompleted();
    void verticalScrollbarMoved(int value);
    void horizontalScrollbarMoved(int value);
};

}
#endif // RASTERVIEW_H
