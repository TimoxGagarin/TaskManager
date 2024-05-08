#ifndef LOADCUSTOMPLOT_H
#define LOADCUSTOMPLOT_H
#include <qcustomplot.h>


class LoadCustomPlot: public QCustomPlot
{
private:
    QVector<QColor> colours = {
            QColor(Qt::red),
            QColor(Qt::green),
            QColor(Qt::blue),
            QColor(Qt::cyan),
            QColor(Qt::magenta),
            QColor(Qt::yellow),
            QColor(Qt::darkRed),
            QColor(Qt::darkGreen),
            QColor(Qt::darkBlue),
            QColor(Qt::darkCyan),
            QColor(Qt::darkMagenta),
            QColor(Qt::darkYellow)
        };
public:
    LoadCustomPlot(QWidget *parent = nullptr);
};

#endif // LOADCUSTOMPLOT_H
