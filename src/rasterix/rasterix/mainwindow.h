/****************************************************************************
**
** File: mainwindow.h.
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QProgressBar>
#include <QLabel>
#include <QTemporaryDir>
#include <QButtonGroup>
#include "../rasterixplugin/tool.h"
#include "gdal.h"
#include "gdal_priv.h"

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionPlugins_triggered();
    void save();
    void saveAs();
    void activateTool();
    void onRasterViewUpdateStarted();
    void onRasterViewUpdateCompleted();
    void onToolStarted();
    void onToolCompleted();
    void onToolError(QString message);
    void fetchProcessedDatasetFromWorker(Rasterix::Worker* worker);

    void on_stackedWidget_currentChanged(int arg1);

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QLabel _statusLabel;
    QProgressBar _progressBar;
    GDALDataset* _workingDataset;
    QString _currentOpenFileName;
    QString _currentTempFileName;
    QTemporaryDir _scratchDir;
    QList<Rasterix::Tool*> _tools;
    QButtonGroup *_tabButtonsGroup;

    void loadPlugins();

    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
