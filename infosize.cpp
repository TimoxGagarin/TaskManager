#include "infosize.h"
#include <math.h>
#include <QDebug>

InfoSize::InfoSize(){

}

InfoSize::InfoSize(long long bytes){
    units_list = QStringList() << "B" << "KB" << "MB" << "GB" << "TB";
    setBytes(bytes);
}

InfoSize::InfoSize(QString str){
    num = str.split(' ')[0].toDouble();
    unit = str.split(' ')[1];
    bytes = (long long)num * pow(1024.0f, units_list.indexOf(unit));
}

void InfoSize::setBytes(long long bytes){
    this->bytes = bytes;
    num = (float)bytes;
    QStringListIterator i(units_list);
    unit = i.next();
    for(; num >= 1024.0 && i.hasNext(); unit = i.next())
        num /= 1024.0;
}

QString InfoSize::toString(){
    return (QString::number(num, 'g', 4) + " " + unit);
}

bool InfoSize::operator==(const InfoSize& other) const{
    return bytes == other.bytes;
}

bool InfoSize::operator!=(const InfoSize& other) const{
    return bytes != other.bytes;
}

bool InfoSize::operator<(const InfoSize& other) const{
    return bytes < other.bytes;
}

bool InfoSize::operator>(const InfoSize& other) const{
    return bytes > other.bytes;
}

bool InfoSize::operator<=(const InfoSize& other) const{
    return bytes <= other.bytes;
}

bool InfoSize::operator>=(const InfoSize& other) const{
    return bytes >= other.bytes;
}
