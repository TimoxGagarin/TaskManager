#include "model.h"

Model::Model()
{

}

Model::~Model(){
    name.clear();
}

Model::Model(QString name)
{
    this->name = name;
}
