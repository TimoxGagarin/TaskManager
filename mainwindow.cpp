#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "procinfo.h"
#include "filesystem.h"
#include "fsqtablewidget.h"
#include "worker.h"
#include "squarelegenditem.h"

#include <qdebug.h>
#include <qlist.h>
#include <qstringlist.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QAction>
#include <qlineedit.h>

#include <pthread.h>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Подготовка рабочего потока
    workerThread = new QThread();
    worker = new Worker();
    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, &Worker::timer_updateTables);
    connect(worker, &Worker::process_updated, this, &MainWindow::updateProcesses);
    connect(worker, &Worker::fileSystems_updated, this, &MainWindow::updateFileSystems);

    workerPlotThread = new QThread();
    plotWorker = new Worker();
    plotWorker->moveToThread(workerPlotThread);

    connect(workerPlotThread, &QThread::started, plotWorker, &Worker::timer_updatePlots);
    connect(plotWorker, &Worker::updateCpuPlotSIG, this, &MainWindow::updateCpuPlotSLO);

    ui->setupUi(this);

    connect(ui->searchLineEdit, &QLineEdit::textChanged, [&](QString str){ui->processTableWidget->filterTable(str);});
    connect(ui->processTableWidget, &QTableWidget::customContextMenuRequested, ui->processTableWidget, &ProcQTableWidget::showContextMenu);
}

void MainWindow::showEvent(QShowEvent *event){
    workerThread->start();
    workerPlotThread->start();
    QMainWindow::showEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event){
    worker->stop();
    plotWorker->stop();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateProcesses(QList<ProcInfo> list){
    ui->processTableWidget->setSortingEnabled(false);
    ui->processTableWidget->setUpdatesEnabled(false);
    ui->processTableWidget->clearContents();
    ui->processTableWidget->setRowCount(0);

    for(auto el: list)
        ui->processTableWidget->addItem(&el);
    ui->processTableWidget->filterTable(ui->tabWidget->widget(0)->findChild<QLineEdit*>()->text());
    ui->processTableWidget->setUpdatesEnabled(true);
    ui->processTableWidget->setSortingEnabled(true);
}

void MainWindow::updateFileSystems(QList<FileSystem> list){
    ui->fileSystemTableWidget->setSortingEnabled(false);
    ui->fileSystemTableWidget->setUpdatesEnabled(false);

    ui->fileSystemTableWidget->clearContents();
    ui->fileSystemTableWidget->setRowCount(0);
    for(auto el: list)
        ui->fileSystemTableWidget->addItem(&el);
    ui->fileSystemTableWidget->setUpdatesEnabled(true);
    ui->fileSystemTableWidget->setSortingEnabled(true);
}

void MainWindow::updateCpuPlotSLO(const QVector<QVector<double>> &input)
{
    QVector<double> x(60);
    for (int i=59; i>0; --i)
        x[59-i] = i;
    const QVector<QVector<double>> *values = &input;
    for(int i=0; i<values->count(); i++) {
        ui->cpuGraph->graph(i)->data()->clear();
        ui->cpuGraph->graph(i)->setData(x, values->at(i));
        ((SquareLegendItem *)ui->cpuGraph->legend->item(i))->setVal(QString::number(values->at(i)[59], 'g', 4) + "%");
    }
    ui->cpuGraph->replot();
}

void MainWindow::on_processTableWidget_cellActivated(int row, int column)
{

}


void MainWindow::on_fileSystemTableWidget_cellActivated(int row, int column)
{

}

