#include "procqtablewidget.h"
#include <QHeaderView>
#include <qdebug.h>
#include <qmenu.h>
#include <qmessagebox.h>

void ProcQTableWidget::addItem(Model* model)
{
    int rows = rowCount();
    setRowCount(rows + 1);
    ProcInfo* procInfo = (ProcInfo*)model;
    addCol(columns::name, rows, procInfo->getName());
    addCol(columns::pid, rows, procInfo->getPid());
    addCol(columns::ppid, rows, procInfo->getPpid());
    addCol(columns::state, rows, procInfo->getState());
    addCol(columns::cmd, rows, procInfo->getCmd());
}

Model* ProcQTableWidget::getItem(int row){
    ProcInfo* ret = new ProcInfo;
    ret->setName(item(row, columns::name)->text());
    ret->setPid(item(row, columns::pid)->text().toInt());
    ret->setPpid(item(row, columns::ppid)->text().toInt());
    ret->setState(item(row, columns::state)->text());
    ret->setCmd(item(row, columns::cmd)->text());
    return (Model*) ret;
}

void ProcQTableWidget::showContextMenu(const QPoint &pos){
    QPoint globalPos = mapToGlobal(pos);
    QMenu menu;

    QMessageBox msgBox;
    msgBox.setText("Hello, world!");
    msgBox.setInformativeText("This is an informative text.");

    menu.addAction("Свойства", this, [&](){ msgBox.exec();});
    menu.addSeparator();
    menu.addAction("Остановить", this, [&](){ ((ProcInfo*)getItem(currentRow()))->stopProc(); });
    menu.addAction("Продолжить", this, [&](){((ProcInfo*)getItem(currentRow()))->continueProc();});
    menu.addAction("Завершить", this, [&](){((ProcInfo*)getItem(currentRow()))->closeProc();});
    menu.addAction("Прервать", this, [&](){((ProcInfo*)getItem(currentRow()))->interruptProc();});

    menu.exec(globalPos);
}

void ProcQTableWidget::filterTable(QString str){
    QList<int> colsToFind = QList<int>() << columns::name << columns::pid << columns::cmd;
    for (int i = 0; i < rowCount(); ++i)
        setRowHidden(i, true);
    for (int i = 0; i < rowCount(); ++i) {
        for (QList<int>::iterator j = colsToFind.begin(); j != colsToFind.end(); ++j) {
            if (item(i, *j)->text().contains(str, Qt::CaseInsensitive)) {
                setRowHidden(i, false);
                break;
            }
        }
    }
}
