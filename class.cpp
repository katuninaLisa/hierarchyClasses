#include "class.h"

Class::Class() {}

Class::Class(const QString &n)
{
    className = n;
}

Class::Class (const QString&name, Property& prop)
{
    className = name;
    properties.insert(prop);
}

QString Class::getName() const
{
    return className;
}

QSet<Property> Class::getProperties() const
{
    return properties;
}

bool Class::operator==(const Class& other) const
{
    return (className == other.className && properties == other.properties);
}

bool Class::operator!=(const Class& other) const
{
    if (className != other.className || properties != other.properties)
        return true;
}

bool Class::operator=(Class& other)
{
    className = other.className;
    properties = other.properties;
}

void Class::setclassName(QString name)
{
    className = name;
}

void Class::setproperties(QSet<Property> prop)
{
    properties = prop;
}

bool Class::isSubClass(const Class &other) const
{
    for (const Property &superClass : other.properties)
    {
        bool found = false; //считаем, что не нашли в подклассе свойства, содержащиеся в родительском
        for (const Property &subClass: properties)
        {
            if (superClass.valuePropertyExist())
            {
                if (subClass == superClass)
                    found = true;
            }
            else
            {
                if (subClass.getPropertyName() == superClass.getPropertyName())
                    found = true;
            }
        }
        if (!found)
            return false;
    }
    return true;
}
