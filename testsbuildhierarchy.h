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
    void emptyClasses(); // Пустой список классов
    void singleClass(); // Один класс без наследников
    void twoIndependentClasses(); // Два независимых класса
    void simpleInheritance(); // Один класс наследует другой
    void chainInheritance(); // Цепочка наследования
    void multipleInheritance(); // Множественное наследования
    void multiLevelInheritance(); // Несколько уровней наследования
    void multipleHierarchies(); // Несколько иерархий классов
};

#endif // TESTSBUILDHIERARCHY_H
