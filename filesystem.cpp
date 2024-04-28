#include "filesystem.h"
#include <qstring.h>

FileSystem::FileSystem(){

}

FileSystem::~FileSystem(){
    name.clear();
    mount_dir.clear();
    type.clear();
}

FileSystem::FileSystem(QString name, QString mount_dir, QString type, InfoSize total, InfoSize free, InfoSize available, InfoSize used) : Model(name) ,total(total), free(free), available(available), used(used){
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
            this->total.toString()  + ", " +
            this->free.toString()  + ", " +
            this->available.toString()  + ", " +
            this->used.toString();
}
