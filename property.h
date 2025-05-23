#ifndef PROPERTY_H
#define PROPERTY_H
#include <QSet>
#include <QMap>

class Property
{
public:
    Property();
    QString getPropertyName() const;
    QVector<int> getValues() const;
    void setpropertyName(QString propert);
    void setvalues(int val);

    bool operator==(const Property& other) const;
    bool valuePropertyExist() const;

    Property (const QString &name, const QVector <int> &val);
    Property (const QString &name, int val);
    Property (const QString &name);

private:
    QString propertyName;
    QVector <int> values;
};

uint qHash(const Property &key, uint seed = 0);

#endif // PROPERTY_H
