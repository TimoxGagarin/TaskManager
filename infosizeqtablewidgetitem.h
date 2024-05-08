#ifndef INFOSIZEQTABLEWIDGETITEM_H
#define INFOSIZEQTABLEWIDGETITEM_H
#include <QTableWidgetItem>
#include "infosize.h"
#include <qdebug.h>


class InfoSizeQTableWidgetItem: public QTableWidgetItem
{
public:
    InfoSizeQTableWidgetItem(QString str = QString("0")):QTableWidgetItem(str) {}

    bool operator <(const QTableWidgetItem &other) const {
        QString str1 = text();
        QString str2 = other.text();
        long long bytes1 = InfoSize(str1).getBytes();
        long long bytes2 = InfoSize(str2).getBytes();
        return bytes1 < bytes2;
    }
};

#endif // INFOSIZEQTABLEWIDGETITEM_H
