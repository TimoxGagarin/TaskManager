#ifndef MODELQTABLEWIDGET_H
#define MODELQTABLEWIDGET_H
#include <QTableWidget>
#include <QStringList>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <qdebug.h>
#include <QHeaderView>
#include <type_traits>

#include "infosize.h"
#include "infosizeqtablewidgetitem.h"
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
    ModelQTableWidget(QWidget *parent = nullptr);

    virtual void addItem(Model* model) = 0;
    virtual Model* getItem(int row) = 0;

    template<typename T>
    void addCol(int col, int rows, T el){
        QTableWidgetItem* item;
        if constexpr(std::is_same<T, InfoSize>()){
            item = new InfoSizeQTableWidgetItem();
            item->setText(el.toString());
        }
        else{
            item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, QVariant::fromValue(el));
        }
        setItem(rows, col, item);
    }

    void sortByColumn(int column);
    void setStyles(QTableWidgetItem *item);
};
#endif // MODELQTABLEWIDGET_H
