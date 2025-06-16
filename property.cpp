#include "property.h"

Property::Property() {}

Property::Property(const QString &name, const QVector<int> &val)
{
    propertyName = name;
    values = val;
}

Property::Property(const QString &name, int val)
{
    propertyName = name;
    values.append(val);
}

Property::Property (const QString &name)
{
    propertyName = name;
}

QString Property::getPropertyName() const
{
    return propertyName;
}

QVector<int> Property::getValues() const
{
    return values;
}

bool Property::operator==(const Property& other) const
{
    return propertyName == other.propertyName && values == other.values;
}

bool Property::valuePropertyExist() const
{
    if (values.size() != 0)
        return true;
    return false;
}

void Property::setpropertyName(QString propert)
{
    propertyName = propert;
}

void Property::setvalues(int val)
{
    values.append(val);
}

