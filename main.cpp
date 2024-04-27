#include "mainwindow.h"
#include <qlist.h>
#include "procinfo.h"
#include "filesystem.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    qRegisterMetaType<QList<ProcInfo>>("QList<ProcInfo>");
    qRegisterMetaType<QList<FileSystem>>("QList<FileSystem>");

    QThread workerThread;
    Worker worker;
    worker.moveToThread(&workerThread);
    QObject::connect(&workerThread, &QThread::started, &worker, &Worker::timer_updateTables);
    QObject::connect(&worker, &Worker::process_updated, &w, &MainWindow::updateProcesses);
    QObject::connect(&worker, &Worker::fileSystems_updated, &w, &MainWindow::updateFileSystems);
    QObject::connect(&w, &MainWindow::closeEvent, &workerThread, &QThread::quit);

    workerThread.start();
    w.show();
    workerThread.wait();
    return a.exec();
}
