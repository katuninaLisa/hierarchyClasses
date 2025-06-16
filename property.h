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

inline uint qHash(const Property &key, uint seed = 0) {
    uint hash = qHash(key.getPropertyName(), seed);
    QVector<int> values = key.getValues();
    for (int i = 0; i < values.size(); ++i) {
        hash ^= qHash(values.at(i), hash);
    }
    return hash;
}

#endif // PROPERTY_H
