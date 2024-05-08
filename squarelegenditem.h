#ifndef SQUARELEGENDITEM_H
#define SQUARELEGENDITEM_H
#include "qcustomplot.h"

class SquareLegendItem : public QCPPlottableLegendItem {
private:
    QColor brush;
    QString name;
    QString val;
public:
    SquareLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable, QColor brush, QString name, QString val)
        : QCPPlottableLegendItem(parent, plottable), brush(brush), name(name), val(val) {}

    virtual void draw(QCPPainter *painter) override;
    void setVal(QString val) {this->val = val;};

};

#endif // SQUARELEGENDITEM_H
