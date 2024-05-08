#ifndef PROCQTABLEWIDGET_H
#define PROCQTABLEWIDGET_H
#include <QTableWidget>
#include <QStringList>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <qdebug.h>
#include <QHeaderView>

#include "procinfo.h"
#include "model.h"
#include "modelqtablewidget.h"

class ProcQTableWidget: public ModelQTableWidget
{
protected:
    enum columns {
        name,
        pid,
        cpu,
        ppid,
        virtual_memory,
        resident_memory,
        cmd,
        state,
        start_time,
        priority,
    };
public:
    ProcQTableWidget(QWidget *parent = nullptr): ModelQTableWidget(parent) {};
    void addItem(Model* model) override;
    void showContextMenu(const QPoint &pos);
    void filterTable(QString str);
    Model* getItem(int row) override;
};

#endif // PROCQTABLEWIDGET_H
