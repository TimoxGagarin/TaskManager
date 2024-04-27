#include "procqtablewidget.h"
#include <QHeaderView>
#include <qdebug.h>
#include <qmenu.h>

void ProcQTableWidget::addItem(Model* model)
{
    int rows = rowCount();
    setRowCount(rows + 1);
    ProcInfo* procInfo = (ProcInfo*)model;
    addCol(columns::name, rows, procInfo->getName());
    addCol(columns::pid, rows, procInfo->getPid());
    addCol(columns::ppid, rows, procInfo->getPpid());
    addCol(columns::state, rows, procInfo->getState());
}

Model* ProcQTableWidget::getItem(int row){
    ProcInfo* ret = new ProcInfo;
    ret->setName(item(row,0)->text());
    ret->setPid(item(row,1)->text().toInt());
    ret->setPpid(item(row,2)->text().toInt());
    ret->setState(item(row,3)->text());
    return (Model*) ret;
}

void ProcQTableWidget::showContextMenu(const QPoint &pos){
    QPoint globalPos = mapToGlobal(pos);
    QMenu menu;

    menu.addAction("Остановить", this, [&](){ ((ProcInfo*)getItem(currentRow()))->stopProc(); });
    menu.addAction("Продолжить", this, [&](){((ProcInfo*)getItem(currentRow()))->continueProc();});
    menu.addAction("Завершить", this, [&](){((ProcInfo*)getItem(currentRow()))->closeProc();});
    menu.addAction("Прервать", this, [&](){((ProcInfo*)getItem(currentRow()))->interruptProc();});

    menu.exec(globalPos);
}
