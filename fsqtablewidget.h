#ifndef FSQTABLEWIDGET_H
#define FSQTABLEWIDGET_H

#include <QTableWidget>
#include <QStringList>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <qdebug.h>
#include <QHeaderView>

#include "model.h"
#include "filesystem.h"
#include "modelqtablewidget.h"

class FSQTableWidget: public ModelQTableWidget
{
protected:
    enum columns {
        name,
        mount_dir,
        type,
        total,
        free,
        available,
        used,
    };
public:
    FSQTableWidget(QWidget *parent = nullptr): ModelQTableWidget(parent) {};
    void addItem(Model* model) override;
    Model* getItem(int row) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // FSQTABLEWIDGET_H
