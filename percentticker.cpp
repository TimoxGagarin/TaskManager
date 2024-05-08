#include "percentticker.h"

QString PercentTicker::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) {
    return locale.toString(tick, formatChar.toLatin1(), precision) + "%";
}
