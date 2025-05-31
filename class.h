#ifndef CLASS_H
#define CLASS_H
#include <QSet>
#include "property.h"

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
    bool operator=(Class&other);
    bool isSubClass(const Class&other) const;

private:
    QString className;
    QSet<Property> properties;
};

inline size_t qHash(const Class& key, size_t seed = 0) noexcept
{
    // Хешируем имя класса
    size_t hash = qHash(key.getName(), seed);

    // Добавляем хеш каждого свойства
    for (const Property& prop : key.getProperties()) {
        hash ^= qHash(prop); // Предполагается, что Property уже имеет qHash
    }

    return hash;
}

#endif // CLASS_H
