#include "fsqtablewidget.h"
#include <QHeaderView>
#include <model.h>
#include <qdebug.h>
#include <qprogressbar.h>
#include <QHBoxLayout>
#include <qlabel.h>
#include <QDesktopServices>
#include <QUrl>
#include <math.h>

void FSQTableWidget::addItem(Model* model)
{
    int rows = rowCount();
    setRowCount(rows + 1);
    FileSystem* fileSystem = (FileSystem*)model;
    addCol(columns::name, rows, fileSystem->getName());
    addCol(columns::mount_dir, rows, fileSystem->getMountDir());
    addCol(columns::type, rows, fileSystem->getType());
    addCol(columns::total, rows, InfoSize(fileSystem->getTotal()));
    addCol(columns::free, rows, InfoSize(fileSystem->getFree()));
    addCol(columns::available, rows, InfoSize(fileSystem->getAvailable()));

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue((int)(100*(double)fileSystem->getUsed().getBytes() / (double)fileSystem->getTotal().getBytes()));
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

    QLabel *label = new QLabel(fileSystem->getUsed().toString());

    layout->addWidget(label);
    layout->addWidget(progressBar);
    layout->setSpacing(5);
    layout->setMargin(0);

    setCellWidget(rows, columns::used, widget);
}

Model* FSQTableWidget::getItem(int row){
    FileSystem* ret = new FileSystem;
    ret->setName(item(row, columns::name)->text());
    ret->setMountDir(item(row, columns::mount_dir)->text());
    ret->setType(item(row, columns::type)->text());
    ret->setTotal(InfoSize(item(row, columns::total)->text()));
    ret->setFree(InfoSize(item(row, columns::free)->text()));
    ret->setAvailable(InfoSize(item(row, columns::available)->text()));
    ret->setUsed(InfoSize(cellWidget(row, columns::used)->findChild<QLabel*>()->text()));
    return (Model*) ret;
}

void FSQTableWidget::mouseDoubleClickEvent(QMouseEvent *event){
    int row = currentRow();
    if(row == -1)
        return;
    QString path = ((FileSystem*)getItem(row))->getMountDir();
    QUrl url = QUrl::fromLocalFile(path);
    QDesktopServices::openUrl(url);
    QTableWidget::mouseDoubleClickEvent(event);
}
