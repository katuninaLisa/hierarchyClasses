#ifndef TESTS_CLASSHIERARCHYDOT_H
#define TESTS_CLASSHIERARCHYDOT_H
#include "class.h"
#include "property.h"
#include <QObject>

extern QString classHierarchyDOT(QMap <QString, QMap <QString, int>> &matrix);

class tests_classHierarchyDOT: public QObject
{
    Q_OBJECT
public:
    explicit tests_classHierarchyDOT(QObject *parent = nullptr);

private slots:
    void test_class_hierarchyDOT_data();
    void test_class_hierarchyDOT();
};

typedef QMap <QString, QMap <QString, int>> newQMapName;

#endif // TESTS_CLASSHIERARCHYDOT_H
