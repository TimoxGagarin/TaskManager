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
    units_list = QStringList() << "B" << "KB" << "MB" << "GB" << "TB";

    if(str == "Н/Д"){
        num = 0;
        unit = units_list[0];
        bytes = 0;
        error = "Н/Д";
        return;
    }
    num = str.split(' ')[0].toDouble();
    unit = str.split(' ')[1].toUpper();
    bytes = (long long)(num * pow(1024.0f, units_list.indexOf(unit)));
    setBytes(this->bytes);
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
    if(error.isEmpty())
        return (QString::number(num, 'g', 4) + " " + unit);
    return error;
}
