#include "squarelegenditem.h"
#include <qdebug.h>

void SquareLegendItem::draw(QCPPainter *painter) {
    painter->setPen(QPen(Qt::black));
    painter->setBrush(QBrush(brush));
    QRect rect = this->rect();
    QPoint center = rect.center();

    int size = qMin(rect.width(), rect.height())/2;

    painter->setFont(QFont("Consolas", size*0.3f));
    QRect squareRect(center.x() - 2*size, center.y() - 0.8*size, size, 0.8*size);
    painter->drawText(QRect(center.x() - size/2, center.y() - 0.8*size, 200, 0.8*size), name + "\r\n" + val, QTextOption());
    painter->drawRect(squareRect);
}
