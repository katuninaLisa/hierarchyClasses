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

    QSet <Property> createProperties(QMap <QString, QVector<int>> &prop);
    Property (const QString &name, const QVector <int> &val);

private:
    QString propertyName;
    QVector <int> values;
};

uint qHash(const Property &key, uint seed = 0);

#endif // PROPERTY_H
