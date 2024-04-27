#include "filesystem.h"
#include <qstring.h>

FileSystem::FileSystem()
{

}

FileSystem::~FileSystem(){
    name.clear();
    mount_dir.clear();
    type.clear();
}

FileSystem::FileSystem(QString name, QString mount_dir, QString type, long long total, long long free, long long available, long long used) : Model(name){
    this->mount_dir = mount_dir;
    this->type = type;
    this->total = total;
    this->free = free;
    this->available = available;
    this->used = used;
}

QString FileSystem::toString(){
    return this->name + ", " +
            this->mount_dir  + ", " +
            this->type  + ", " +
            QString::number(this->total)  + ", " +
            QString::number(this->free)  + ", " +
            QString::number(this->available)  + ", " +
            QString::number(this->used);
}
