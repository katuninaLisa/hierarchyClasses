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
    void emptyMatrix(); //Пустая матрица
    void singleClass(); //Один класс без связей
    void twoIndependentClasses(); //Два независимых класса
    void simpleInheritance(); //Простое наследование
    void multipleInheritance(); //Множественное наследование
    void deepHierarchy(); //Глубокая иерархия
    void rhomboidInheritance(); //Ромбовидное наследование
    void someClassHierarchies(); //Несколько иерархий классов
};

#endif // TESTS_CLASSHIERARCHYDOT_H
