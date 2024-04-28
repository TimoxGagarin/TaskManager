#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <qstring.h>
#include "model.h"
#include "infosize.h"

class FileSystem: public Model
{
protected:
    QString mount_dir;
    QString type;
    InfoSize total;
    InfoSize free;
    InfoSize available;
    InfoSize used;
public:
    FileSystem();
    FileSystem(QString name, QString mount_dir, QString type, InfoSize total, InfoSize free, InfoSize available, InfoSize used);
    ~FileSystem();

    QString toString() override;

    QString getMountDir() {return mount_dir;};
    void setMountDir(QString mount_dir) {this->mount_dir = mount_dir;};
    QString getType() {return type;};
    void setType(QString type) {this->type = type;};
    InfoSize getTotal() {return total;};
    void setTotal(InfoSize total) {this->total = total;};
    InfoSize getFree() {return free;};
    void setFree(InfoSize free) {this->free = free;};
    InfoSize getAvailable() {return available;};
    void setAvailable(InfoSize available) {this->available = available;};
    InfoSize getUsed() {return used;};
    void setUsed(InfoSize used) {this->used = used;};
};

#endif // FILESYSTEM_H
