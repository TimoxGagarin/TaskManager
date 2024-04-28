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
        ppid,
        cmd,
        state,
    };
public:
    ProcQTableWidget(int rows, int columns, const QStringList& horizontalHeaders, const QStringList& verticalHeaders, QWidget *parent = nullptr)
        : ModelQTableWidget(rows, columns, horizontalHeaders, verticalHeaders, parent){
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &QWidget::customContextMenuRequested, this, &ProcQTableWidget::showContextMenu);
    };
    void addItem(Model* model) override;
    void showContextMenu(const QPoint &pos);
    void filterTable(QString str);
    Model* getItem(int row) override;
};

#endif // PROCQTABLEWIDGET_H
