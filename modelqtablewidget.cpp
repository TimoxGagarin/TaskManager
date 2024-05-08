#include "modelqtablewidget.h"

ModelQTableWidget::ModelQTableWidget(QWidget *parent): QTableWidget(parent)
{
    sortColumn = -1;
    sortOrder = Qt::AscendingOrder;
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &ModelQTableWidget::sortByColumn);
    horizontalHeader()->setMaximumSectionSize(250);
}

void ModelQTableWidget::sortByColumn(int column) {
    if (sortColumn == column) {
        sortOrder = (sortOrder == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    } else {
        sortColumn = column;
        sortOrder = Qt::AscendingOrder;
    }
    sortItems(column, sortOrder);
    setSortingEnabled(false);
    horizontalHeader()->setSortIndicator(column, sortOrder);
    setSortingEnabled(true);
}
