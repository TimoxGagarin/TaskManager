#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QThread>
#include "procqtablewidget.h"
#include "fsqtablewidget.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    FSQTableWidget *fileSystemsTableWidget;

    QThread* workerThread;
    QThread* workerPlotThread;
    Worker* worker;
    Worker* plotWorker;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void timer_updateTables();
public slots:
    void updateProcesses(QList<ProcInfo> list);
    void updateFileSystems(QList<FileSystem> list);
private slots:
    void on_fileSystemTableWidget_cellActivated(int row, int column);
    void on_processTableWidget_cellActivated(int row, int column);
    void updateCpuPlotSLO(const QVector<QVector<double>> &input);
};
#endif // MAINWINDOW_H
