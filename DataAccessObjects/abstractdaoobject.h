#ifndef ABSTRACTDAOOBJECT_H
#define ABSTRACTDAOOBJECT_H
#include <QJsonObject>
#include <QVariantMap>
class AbstractDAOObject
{

public:
    virtual void fromJsonObject(const QJsonObject & jsonObject) = 0;
    virtual QJsonObject toJsonObject() const
    {
        return QJsonObject::fromVariantMap(map_);
    };

protected:
    QVariantMap map_;
};

#endif // ABSTRACTDAOOBJECT_H
