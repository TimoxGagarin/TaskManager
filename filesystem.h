#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <qstring.h>
#include "model.h"

class FileSystem: public Model
{
protected:
    QString mount_dir;
    QString type;
    long long total;
    long long free;
    long long available;
    long long used;
public:
    FileSystem();
    FileSystem(QString name, QString mount_dir, QString type, long long total, long long free, long long available, long long used);
    ~FileSystem();

    QString toString() override;

    QString getMountDir() {return mount_dir;};
    void setPid(QString mount_dir) {this->mount_dir = mount_dir;};
    QString getType() {return type;};
    void setType(QString type) {this->type = type;};
    long long getTotal() {return total;};
    void setTotal(long long total) {this->total = total;};
    long long getFree() {return free;};
    void setFree(long long free) {this->free = free;};
    long long getAvailable() {return available;};
    void setAvailable(long long available) {this->available = available;};
    long long getUsed() {return used;};
    void setUsed(long long used) {this->used = used;};
};

#endif // FILESYSTEM_H
