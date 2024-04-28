#ifndef MODELQTABLEWIDGET_H
#define MODELQTABLEWIDGET_H
#include <QTableWidget>
#include <QStringList>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <qdebug.h>
#include <QHeaderView>

#include "infosize.h"
#include "model.h"

class ModelQTableWidget: public QTableWidget
{
protected:
    enum columns {
        name,
    };
    int sortColumn;
    Qt::SortOrder sortOrder;
public:
    ModelQTableWidget(int rows, int columns, const QStringList& horizontalHeaders, const QStringList& verticalHeaders, QWidget *parent = nullptr);

    virtual void addItem(Model* model) = 0;
    virtual Model* getItem(int row) = 0;

    template<typename T>
    void addCol(int col, int rows, T el){
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, QVariant::fromValue(el));
        if(typeid(el) == typeid(InfoSize))
            item->setText(((InfoSize)el).toString());
        setItem(rows, col, item);
    }

    void sortByColumn(int column);
    void setStyles(QTableWidgetItem *item);
};
#endif // MODELQTABLEWIDGET_H
