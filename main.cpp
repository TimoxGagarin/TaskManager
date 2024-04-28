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

    w.show();
    return a.exec();
}
