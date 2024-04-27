#ifndef MODEL_H
#define MODEL_H
#include <QString>

class Model
{
protected:
    QString name;
public:
    Model();
    Model(QString name);
    ~Model();

    virtual QString toString() = 0;

    QString getName() {return name;};
    void setName(QString name) {this->name = name;};
};

#endif // MODEL_H
