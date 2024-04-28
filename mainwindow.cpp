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

    /// Загрузка интерфейса
    // Страница "Процессы"
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabWidget::tab-bar {alignment: center;}");
    QLineEdit *searchLineEdit = new QLineEdit();
    QStringList headers = QStringList() << "Name" << "PID" << "PPID" << "CMD" << "State";
    processTableWidget = new ProcQTableWidget(0, headers.size(), headers, {}, this);

    QWidget *tableWidget = new QWidget();
    QVBoxLayout layout(tableWidget);
    layout.addWidget(searchLineEdit);
    layout.addWidget(processTableWidget);
    ui->tabWidget->insertTab(0, tableWidget, "Процессы");

    connect(searchLineEdit, &QLineEdit::textChanged, [&](QString str){processTableWidget->filterTable(str);});

    // Страница "Файловые системы"
    fileSystemsTableWidget = new FSQTableWidget(0, 7, QStringList() << "Name" << "Mounted" << "Type" << "Total" << "Free" << "Available" << "Used", {}, this);
    ui->tabWidget->insertTab(1, fileSystemsTableWidget, "Файловые системы");
}

void MainWindow::showEvent(QShowEvent *event){
    workerThread->start();
    QMainWindow::showEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->tabWidget->setMinimumSize(this->width(), this->height());
    processTableWidget->setMinimumSize(this->width(), this->height());
}

void MainWindow::closeEvent(QCloseEvent *event){
    worker->stop();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateProcesses(QList<ProcInfo> list){
    processTableWidget->setSortingEnabled(false);
    processTableWidget->setUpdatesEnabled(false);
    this->processTableWidget->clearContents();
    this->processTableWidget->setRowCount(0);

    for(auto el: list){
        this->processTableWidget->addItem(&el);
    }
    this->processTableWidget->filterTable(ui->tabWidget->widget(0)->findChild<QLineEdit*>()->text());
    processTableWidget->setUpdatesEnabled(true);
    processTableWidget->setSortingEnabled(true);
}

void MainWindow::updateFileSystems(QList<FileSystem> list){
    fileSystemsTableWidget->setSortingEnabled(false);
    this->fileSystemsTableWidget->setUpdatesEnabled(false);

    this->fileSystemsTableWidget->clearContents();
    this->fileSystemsTableWidget->setRowCount(0);
    for(auto el: list){
        this->fileSystemsTableWidget->addItem(&el);
    }
    this->fileSystemsTableWidget->setUpdatesEnabled(true);
    fileSystemsTableWidget->setSortingEnabled(true);
}
