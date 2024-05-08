#include "loadcustomplot.h"
#include "percentticker.h"
#include "squarelegenditem.h"

LoadCustomPlot::LoadCustomPlot(QWidget *parent): QCustomPlot(parent)
{
    // Plot
    plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(this, "CPU", QFont("sans", 14, QFont::Bold));
    plotLayout()->addElement(0, 0, title);

    // Ox
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s с");
    xAxis->setTicker(timeTicker);
    xAxis->ticker()->setTickCount(6);
    xAxis->setRangeReversed(true);
    xAxis->setRange(0, 60);

    // Oy
    yAxis->setVisible(false);

    yAxis2->setVisible(true);
    QSharedPointer<PercentTicker> percentTicker(new PercentTicker);
    yAxis2->setTicker(percentTicker);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(200, 200, 200));
    xAxis->grid()->setPen(gridPen);
    yAxis2->grid()->setPen(gridPen);
    yAxis2->grid()->setVisible(true);
    yAxis->setRange(0, 100);
    yAxis2->setRange(0, 100);

    legend->setVisible(true);
    plotLayout()->addElement(2, 0, legend);
    legend->setFillOrder(QCPLayoutGrid::foColumnsFirst, true);
    legend->setWrap(4);

    for(int i=0; i<12; i++) {
        QString colName = "CPU" + QString::number(i+1);
        QColor cpuColor = colours[i];
        addGraph();
        QCPPlottableLegendItem *oldItem = legend->itemWithPlottable(graph());
        if (oldItem)
            legend->removeItem(oldItem);
        SquareLegendItem *customLegendItem = new SquareLegendItem(
                legend,
                graph(),
                cpuColor,
                colName,
                "0%"
        );
        legend->addItem(customLegendItem);
        graph(i)->setPen(QPen(QColor(cpuColor)));
    }
}
