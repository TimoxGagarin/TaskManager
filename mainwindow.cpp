#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "procinfo.h"
#include "filesystem.h"
#include "fsqtablewidget.h"
#include "worker.h"

#include <qdebug.h>
#include <qlist.h>
#include <qstringlist.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QAction>

#include <pthread.h>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabWidget::tab-bar {alignment: center;}");

    processTableWidget = new ProcQTableWidget(0, 4, QStringList() << "Name" << "PID" << "PPID" << "State", {}, this);
    ui->tabWidget->insertTab(0, processTableWidget, "Процессы");

    fileSystemsTableWidget = new FSQTableWidget(0, 7, QStringList() << "Name" << "Mounted" << "Type" << "Total" << "Free" << "Available" << "Used", {}, this);
    ui->tabWidget->insertTab(1, fileSystemsTableWidget, "Файловые системы");
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->tabWidget->setMinimumSize(this->width(), this->height());
}

void MainWindow::closeEvent(QCloseEvent *event){
//    workerThread.quit();
//    workerThread.wait();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateProcesses(QList<ProcInfo> list){
    processTableWidget->setUpdatesEnabled(false);
    this->processTableWidget->clearContents();
    this->processTableWidget->setRowCount(0);

    for(auto el: list){
        this->processTableWidget->addItem(&el);
    }
    processTableWidget->setUpdatesEnabled(true);
}

void MainWindow::updateFileSystems(QList<FileSystem> list){
    this->fileSystemsTableWidget->setUpdatesEnabled(false);

    this->fileSystemsTableWidget->clearContents();
    this->fileSystemsTableWidget->setRowCount(0);
    for(auto el: list){
        this->fileSystemsTableWidget->addItem(&el);
    }
    this->fileSystemsTableWidget->setUpdatesEnabled(true);
}
