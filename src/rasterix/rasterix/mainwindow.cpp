/****************************************************************************
**
** File: mainwindow.cpp.
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gdaldriverhelper.h"
#include "worker.h"
#include <QPluginLoader>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringList>
#include <QDebug>
#include <QActionGroup>
#include <QThreadPool>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStaticPlugin>
#include <QToolButton>
#include <QVBoxLayout>
#include "aboutpluginsdialog.h"
#include "aboutdialog.h"
#include "../inspectorplugin/inspectortool.h"
#include "../exportplugin/exporttool.h"
#include "../reacticons/reacticons.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _tabButtonsGroup = new QButtonGroup();

    _workingDataset = nullptr;

    loadPlugins();

    // set icons
    ui->actionAbout->setIcon(Reacticons::getIcon(u8"\uf129"));
    ui->actionBatch->setIcon(Reacticons::getIcon(u8"\uf085"));
    ui->actionClose->setIcon(Reacticons::getIcon(u8"\uf057"));
    ui->actionOpen->setIcon(Reacticons::getIcon(u8"\uf07c"));
    ui->actionPlugins->setIcon(Reacticons::getIcon(u8"\uf1e6"));
    ui->actionUserManual->setIcon(Reacticons::getIcon(u8"\uf128"));
    ui->action_Exit->setIcon(Reacticons::getIcon(u8"\uf00d"));

    _progressBar.setVisible(false);
    _progressBar.setMaximumWidth(120);
    ui->statusBar->addWidget(&_progressBar);
    ui->statusBar->addWidget(&_statusLabel);

    ui->splitter->setStretchFactor(0, 90);
    ui->splitter->setStretchFactor(1, 10);

    connect(ui->rasterView, &Rasterix::RasterView::started, this, &MainWindow::onRasterViewUpdateStarted);
    connect(ui->rasterView, &Rasterix::RasterView::completed, this, &MainWindow::onRasterViewUpdateCompleted);

    // load stylesheet
    QFile stylesheetFile(":/stylesheet.qss");
    if(stylesheetFile.open(QIODevice::ReadOnly))
    {
        QString style = stylesheetFile.readAll();
        stylesheetFile.close();

        style = style.replace("$SELECTION_COLOR$", QApplication::palette().highlight().color().name());
        style = style.replace("$WINDOW_COLOR$", QApplication::palette().window().color().name());
        style = style.replace("$DARK_COLOR$", QApplication::palette().midlight().color().name());
        style = style.replace("$LIGHT_COLOR$", QApplication::palette().dark().color().name());

        setStyleSheet(style);
    }

    ui->rasterView->setEnabled(false);
    ui->stackedWidget->setEnabled(false);
    ui->tabSelector->setEnabled(false);
}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered()
{   
    QStringList rasterFormats = Rasterix::GDALDriverHelper::getRasterReadExtensions();

    rasterFormats.sort();
    rasterFormats.insert(0, tr("All (*.*)"));

    // select files
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open raster file"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    rasterFormats.join(";;"),
                                                    nullptr);

    if (!filename.isEmpty())
    {
        _currentOpenFileName = "";

        // TODO: ask for confirm/save
        // clean up working dataset
        if (_workingDataset != nullptr)
        {
            GDALClose(_workingDataset);
            _workingDataset = nullptr;
        }

        // clean up _temporary folder
        QDir dir(_scratchDir.path());
        if (dir.exists())
        {
            QStringList tmpFiles = dir.entryList();
            for (QString tmp : tmpFiles)
                dir.remove(tmp);
        }

        // open new dataset
        GDALDataset* dataset = (GDALDataset*)GDALOpen(filename.toStdString().data(), GA_ReadOnly);
        if(dataset)
        {
            _currentOpenFileName = filename;
            _workingDataset = dataset;

            for (Rasterix::Tool* t : _tools)
                t->setRaster(_workingDataset);

            ui->rasterView->setDataset(dataset);

            QFileInfo finfo(_currentOpenFileName);
            this->setWindowTitle(QString("Rasterix - %1").arg(finfo.baseName()));

            ui->rasterView->setEnabled(true);
            ui->stackedWidget->setEnabled(true);
            ui->tabSelector->setEnabled(true);
        }
    } 
}

/*!
 * \brief MainWindow::activateTool
 */
void MainWindow::activateTool()
{
    QAction *act = qobject_cast<QAction*>(sender());
    if (act)
    {
        Rasterix::Tool *tool = dynamic_cast<Rasterix::Tool*>((Rasterix::Tool*) (act->data().value<void*>()));

        if (tool)
        {
            // change tab page
            bool ok = false;
            int index = act->property("tab_index").toInt(&ok);
            if (ok) ui->stackedWidget->setCurrentIndex(index);
        }
    }
}

/*!
 * \brief MainWindow::onRasterViewUpdateStarted
 */
void MainWindow::onRasterViewUpdateStarted()
{
    ui->rasterView->setCursor(Qt::WaitCursor);
}

/*!
 * \brief MainWindow::onRasterViewUpdateCompleted
 */
void MainWindow::onRasterViewUpdateCompleted()
{
    ui->rasterView->setCursor(Qt::ArrowCursor);
}

/*!
 * \brief MainWindow::onToolStarted
 */
void MainWindow::onToolStarted()
{
    _progressBar.reset();
    _progressBar.show();

    ui->rasterView->setEnabled(false);
    ui->stackedWidget->setEnabled(false);
    ui->tabSelector->setEnabled(false);
}

/*!
 * \brief MainWindow::onToolCompleted
 */
void MainWindow::onToolCompleted()
{
    Rasterix::Worker* worker = qobject_cast<Rasterix::Worker*>(sender());
    if (worker)
    {
        fetchProcessedDatasetFromWorker(worker);
    }

    _statusLabel.setText(tr("Ready."));
    _progressBar.reset();
    _progressBar.hide();

    ui->rasterView->setEnabled(true);
    ui->stackedWidget->setEnabled(true);
    ui->tabSelector->setEnabled(true);
}

/*!
 * \brief MainWindow::onToolError
 * \param message
 */
void MainWindow::onToolError(QString message)
{
    QMessageBox::critical(this,
                          tr("An error occurred"),
                          message);
}

/*!
 * \brief MainWindow::fetchProcessedDatasetFromWorker
 * \param worker
 */
void MainWindow::fetchProcessedDatasetFromWorker(Rasterix::Worker* worker)
{
    if ((worker != nullptr) && (worker->getProcessedRaster() != nullptr))
    {
        // close old dataset
        if (_workingDataset != nullptr)
        {
            // remove file if it was an old temporary dataset
            // (i.e. it is inside the app's temporary folder)
            char** fileNames = nullptr;
            fileNames = _workingDataset->GetFileList();

            if (fileNames != nullptr)
            {
                // flush to disk
                GDALClose(_workingDataset);

                int i=0;
                while (fileNames[i] != 0)
                {
                    QString fname(fileNames[i]);

                    if (fname.contains(_scratchDir.path()))
                    {
                        QFile item(fname);
                        item.remove();
                    }

                    i++;
                }

                // clean up files list
                CSLDestroy(fileNames);
            }

            _workingDataset = nullptr;
        }

        // get processed dataset
        GDALDataset *newDataset = worker->getProcessedRaster();
        if (newDataset != nullptr)
        {
            // flush to disk newly created dataset
            GDALClose(newDataset);

            // reopen from temporary location
            _currentTempFileName = worker->getProcessedDatasetName();
            _workingDataset = (GDALDataset*)GDALOpen(_currentTempFileName.toStdString().data(), GA_ReadOnly);

            // assign dataset to every tool and to raster view
            for (Rasterix::Tool* t : _tools)
                t->setRaster(_workingDataset);

            ui->rasterView->setDataset(_workingDataset);
        }
    }
}

/*!
 * \brief MainWindow::loadPlugins
 */
void MainWindow::loadPlugins()
{
    QVBoxLayout *tabLayout = new QVBoxLayout();
    tabLayout->setSpacing(0);
    tabLayout->setContentsMargins(0, 0, 0, 0);

    QActionGroup* sidebarToolGroup = new QActionGroup(this);
    foreach (QStaticPlugin toolPlugin, QPluginLoader::staticPlugins())
    {        
        Rasterix::Tool* tool = qobject_cast<Rasterix::Tool*>(toolPlugin.instance());

        if (tool)
        {
            // store plugin metadata inside tool
            tool->setMetadata(toolPlugin.metaData());

            // setup sidebar tools
            if (tool->widget() != nullptr)
            {
                // create a tab for each sidebar tool
                QWidget *container = new QWidget();
                QVBoxLayout *containerLayout = new QVBoxLayout();
                containerLayout->setSpacing(0);
                containerLayout->setContentsMargins(0, 0, 0, 0);

                QLabel *caption = new QLabel();
                caption->setText(tool->name());
                caption->setObjectName("captionLabel");

                containerLayout->addWidget(caption);
                containerLayout->addWidget(tool->widget());

                container->setLayout(containerLayout);

                int index = ui->stackedWidget->addWidget(container);

                // selection tool button
                QToolButton *button = new QToolButton();
                button->setIcon(tool->icon());
                button->setText(tool->name());
                button->setToolTip(tool->name());
                button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                button->setAutoRaise(true);
                button->setCheckable(true);
                button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                button->setIconSize(QSize(32, 32));
                connect(button, &QToolButton::clicked, this, [this, button, index](){
                    ui->stackedWidget->setCurrentIndex(index);
                    button->setChecked(true);
                });
                tabLayout->addWidget(button);
                _tabButtonsGroup->addButton(button);

                // associate tool actions to the corresponding tool's tab page
                QList<QAction*> actions = tool->toolActions();
                for (auto act : actions)
                {
                    if (act != nullptr)
                    {
                        act->setProperty("tab_index", index);

                        sidebarToolGroup->addAction(act);
                        connect(act, &QAction::triggered, this, &MainWindow::activateTool);
                    }
                }

                // add dedicated toolbar if available
                QToolBar* toolbar = tool->toolBar();
                if (toolbar != nullptr)
                {
                    addToolBar(toolbar);
                }
            }

            // create Save and Save as actions from export plugin
            if (qobject_cast<ExportTool*>(tool))
            {
                QList<QAction*> exportActions = tool->toolActions();

                ui->menufile->insertActions(ui->actionClose, exportActions);
                ui->menufile->insertSeparator(ui->actionClose);

                ui->mainToolBar->insertActions(ui->actionBatch, exportActions);
                ui->mainToolBar->insertSeparator(ui->actionBatch);

                if (exportActions.length() == 2)
                {
                    connect(exportActions.at(0), &QAction::triggered, this, &MainWindow::save);
                    connect(exportActions.at(1), &QAction::triggered, this, &MainWindow::saveAs);
                }
            }

            Rasterix::Worker* worker = tool->worker();
            if (worker)
            {
                // connect worker signals to UI
                connect(worker, &Rasterix::Worker::started, this, &MainWindow::onToolStarted);
                connect(worker, &Rasterix::Worker::progress, &_progressBar, &QProgressBar::setValue);
                connect(worker, &Rasterix::Worker::completed, this, &MainWindow::onToolCompleted);
                connect(worker, &Rasterix::Worker::jobDescriptionUpdated, &_statusLabel, &QLabel::setText);
                connect(worker, &Rasterix::Worker::error, this, &MainWindow::onToolError);

                // assign temporary forlder path
                worker->setTempDirectoryPath(_scratchDir.path());
            }

            _tools.append(tool);
        }
    }

    ui->menuTools->addActions(sidebarToolGroup->actions());
    sidebarToolGroup->setExclusive(true);

    tabLayout->addStretch(20);

    ui->tabSelector->setLayout(tabLayout);
    ui->stackedWidget->setCurrentIndex(0);
    on_stackedWidget_currentChanged(0);
}

/*!
 * \brief MainWindow::closeEvent
 * \param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::question(this, tr("Confirm"),
                                       tr("Are you sure you want to close Rasterix?"),
                                       QMessageBox::Yes, QMessageBox::No);
    if(result == QMessageBox::No)
    {
        event->ignore();
    }
}

/*!
 * \brief MainWindow::save
 */
void MainWindow::save()
{
    QAction *act = qobject_cast<QAction*>(sender());
    if (act)
    {
        ExportTool *exportTool = qobject_cast<ExportTool*>((Rasterix::Tool*) (act->data().value<void*>()));

        if (exportTool)
        {
            if (!_currentOpenFileName.isEmpty() && _workingDataset != nullptr)
            {
                exportTool->setRaster(_workingDataset);
                exportTool->setOutputFileName(_currentOpenFileName);
                exportTool->setDriverName(Rasterix::GDALDriverHelper::getLongName(_workingDataset->GetDriver()));
                exportTool->process();
            }
        }
    }
}

/*!
 * \brief MainWindow::saveAs
 */
void MainWindow::saveAs()
{
    QAction *act = qobject_cast<QAction*>(sender());
    if (act)
    {
        ExportTool *exportTool = qobject_cast<ExportTool*>((Rasterix::Tool*) (act->data().value<void*>()));

        if (exportTool)
        {
            QStringList rasterFormats = Rasterix::GDALDriverHelper::getRasterWriteExtensions();
            rasterFormats.sort();    

            // select out raster format
            QString defaultFilter = "GeoTIFF (*.tif)";
            QString filename = QFileDialog::getSaveFileName(this,
                                                            tr("Save as"),
                                                            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                            rasterFormats.join(";;"),
                                                            &defaultFilter);

            QString driverLongName = defaultFilter.left(defaultFilter.indexOf(" (")).trimmed();
            QString ext = defaultFilter.mid(defaultFilter.indexOf("*.") + 2).remove(")").trimmed();

            if (!filename.isEmpty() && _workingDataset != nullptr)
            {
                exportTool->setRaster(_workingDataset);

                // force file extension
                if (!ext.isEmpty() && !filename.endsWith(ext))
                    filename = QString("%1.%2").arg(filename, ext);

                exportTool->setOutputFileName(filename);

                // get GDAL driver name from selected filter.
                exportTool->setDriverName(driverLongName);
                exportTool->process();

                // update current open file name
                _currentOpenFileName = filename;
                QFileInfo finfo(_currentOpenFileName);
                this->setWindowTitle(QString("Rasterix - %1").arg(finfo.baseName()));
            }
        }
    }
}

/*!
 * \brief MainWindow::on_actionPlugins_triggered
 */
void MainWindow::on_actionPlugins_triggered()
{
    AboutPluginsDialog about;
    about.init(_tools);

    about.exec();
}

/*!
 * \brief MainWindow::on_stackedWidget_currentChanged
 * \param index
 */
void MainWindow::on_stackedWidget_currentChanged(int index)
{
    QList<QAction*> toolsActions = ui->menuTools->actions();
    if (!toolsActions.isEmpty() &&
            index >= 0 &&
            index < toolsActions.length())
    {
        ui->menuTools->actions().at(index)->trigger();
    }

    if(_tabButtonsGroup != nullptr)
    {
        if(index >=0 && index < _tabButtonsGroup->buttons().size())
        {
            QAbstractButton *button = _tabButtonsGroup->buttons().at(index);
            if(button)
            {
                button->setChecked(true);
            }
        }
    }
}

/*!
 * \brief MainWindow::on_actionAbout_triggered
 */
void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog;
    dialog.exec();
}
