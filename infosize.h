#ifndef INFOSIZE_H
#define INFOSIZE_H
#include <qstring.h>
#include <qstringlist.h>
#include <qmetatype.h>

class InfoSize
{
private:
    long long bytes;
    double num;
    QString unit;
    QString error;

    QStringList units_list;
public:
    InfoSize();
    InfoSize(QString str);
    InfoSize(long long bytes);

    QString toString();
    void setBytes(long long bytes);
    long long getBytes() { return bytes; };

    double getNum() {return num;};
    void setNum(double num) {this->num = num;};
    QString getUnit() {return unit;};
    void setUnit(QString unit) {this->unit = unit;};
    QString getError() {return error;};
    void setError(QString error) {this->error = error;};
};
Q_DECLARE_METATYPE(InfoSize)
#endif // INFOSIZE_H
