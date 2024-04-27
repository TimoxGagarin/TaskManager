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
    FSQTableWidget(int rows, int columns, const QStringList& horizontalHeaders, const QStringList& verticalHeaders, QWidget *parent = nullptr)
        : ModelQTableWidget(rows, columns, horizontalHeaders, verticalHeaders, parent){};

    void addItem(Model* model) override;
    Model* getItem(int row) override;
};

#endif // FSQTABLEWIDGET_H
