#ifndef CLASS_H
#define CLASS_H
#include <QSet>
#include "property.h"
#include <QMetaType>

class Class
{
public:
    Class();
    Class (const QString&);
    Class (const QString&, Property& prop);

    QString getName() const;
    QSet<Property> getProperties() const;
    void setclassName(QString name);
    void setproperties(QSet <Property> prop);

    bool operator==(const Class&other) const;
    bool operator!=(const Class&other) const;
    bool operator=(const Class&other);
    bool isSubClass(const Class&other) const;

private:
    QString className;
    QSet<Property> properties;
};

inline uint qHash(const Class &key, uint seed = 0) {
    uint hash = qHash(key.getName(), seed);

    QSet<Property> properties = key.getProperties();
    QSet<Property>::const_iterator it;
    for (it = properties.constBegin(); it != properties.constEnd(); ++it) {
        const Property &prop = *it;
        hash ^= qHash(prop, hash);  // Явно передаем текущий hash как seed
    }

    return hash;
}

Q_DECLARE_METATYPE(Class)

#endif // CLASS_H
