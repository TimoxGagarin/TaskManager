#include "fsqtablewidget.h"
#include <QHeaderView>
#include <model.h>
#include <qdebug.h>
#include <qprogressbar.h>
#include <QHBoxLayout>
#include <qlabel.h>

QString size_human(long long in_num)
{
    float num = (float)in_num;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
     {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num,'f',2)+" "+unit;
}


void FSQTableWidget::addItem(Model* model)
{
    int rows = rowCount();
    setRowCount(rows + 1);
    FileSystem* fileSystem = (FileSystem*)model;
    addCol(columns::name, rows, fileSystem->getName());
    addCol(columns::mount_dir, rows, fileSystem->getMountDir());
    addCol(columns::type, rows, fileSystem->getType());
    addCol(columns::total, rows, size_human(fileSystem->getTotal()));
    addCol(columns::free, rows, size_human(fileSystem->getFree()));
    addCol(columns::available, rows, size_human(fileSystem->getAvailable()));

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue((int)(100*(double)fileSystem->getUsed() / (double)fileSystem->getTotal()));
    progressBar->setStyleSheet(
            "QProgressBar {"
            "   border: 0;"
            "   border-radius: 0;"
            "   background-color: #DDDDDD;"
            "   text-align: center;"
            "   margin-right: 20px;"
            "}"
            "QProgressBar::chunk {"
            "   background-color: #FF4800;"  // Цвет заполнения
            "   width: 20px;"  // Ширина отдельного блока заполнения
    "}");

    QLabel *label = new QLabel(size_human(fileSystem->getUsed()));

    layout->addWidget(label);
    layout->addWidget(progressBar);
    layout->setSpacing(5);
    layout->setMargin(0);

    setCellWidget(rows, columns::used, widget);
}

Model* FSQTableWidget::getItem(int row){
    FileSystem* ret = new FileSystem;
    // TODO: fill
    return (Model*) ret;
}
