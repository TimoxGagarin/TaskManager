#include "modelqtablewidget.h"

ModelQTableWidget::ModelQTableWidget(int rows, int columns, const QStringList& horizontalHeaders, const QStringList& verticalHeaders, QWidget *parent)
: QTableWidget(rows, columns, parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableWidget::item:selected { background-color: #FF4800; color: white} QHeaderView::section {background-color: #FEFEFE; padding: 4px; border: 1px solid #F8F8F8;}");

    verticalHeader()->hide();
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setHorizontalHeaderLabels(horizontalHeaders);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setShowGrid(false);

    sortColumn = -1;
    sortOrder = Qt::AscendingOrder;
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &ModelQTableWidget::sortByColumn);
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
