#ifndef PROPERTY_H
#define PROPERTY_H
#include <QSet>

class Property
{
public:
    Property();
    QString getPropertyName() const;
    QVector<int> getValues() const;
    void setpropertyName(QString propert);
    void setvalues(QVector<int> val);

    bool operator==(const Property& other) const;
    bool valuePropertyExist() const;

private:
    QString propertyName;
    QVector <int> values;
};

uint qHash(const Property &key, uint seed = 0);

#endif // PROPERTY_H
