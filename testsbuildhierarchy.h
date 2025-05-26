#ifndef TESTSBUILDHIERARCHY_H
#define TESTSBUILDHIERARCHY_H
#include <QObject>
#include "class.h"
#include "property.h"
#include <QVector>

extern void buildHierarchy(const QSet<Class>&, QMap<QString, QMap<QString, int>>&);

class testsBuildHierarchy : public QObject
{
    Q_OBJECT
public:
    explicit testsBuildHierarchy(QObject *parent = nullptr);

private:
    // Вспомогательный метод
    QSet<Property> createProperties(const QMap<QString, QVector<int>>& props);

private slots:
    void testBuildHierarchy_data();
    void testBuildHierarchy();
};

typedef QMap <QString, QMap <QString, int>> newQMapName;
typedef QSet<Class> newQSetName;


#endif // TESTSBUILDHIERARCHY_H
