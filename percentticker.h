#ifndef PERCENTTICKER_H
#define PERCENTTICKER_H
#include <qcustomplot.h>


class PercentTicker : public QCPAxisTicker {
public:
    QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision);
};

#endif // PERCENTTICKER_H
