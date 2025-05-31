#include "tests_classhierarchydot.h"
#include <QtTest>

tests_classHierarchyDOT::tests_classHierarchyDOT(QObject *parent) : QObject(parent) {}

void tests_classHierarchyDOT::tests_data()
{
    //Создаем колонки-параметры метода
    QTest::addColumn<newQMapName>("matrix");
    QTest::addColumn<QStringList>("expResult");

    //Создаем строки-тесты
    //Пустая матрица
    QTest::newRow("emptyMatrix") << newQMapName () << QStringList {"digraph ClassHierarchy {", "}"};

    //Один класс без связей
    newQMapName singleClass;
    singleClass["животное"] = {{"животное", 0}};
    QTest::newRow("singleClass") << singleClass << QStringList {"digraph ClassHierarchy {", "животное;", "}"};

    //Два независимых класса
    newQMapName twoIndependentClasses ={{"машина", {{"машина", 0},{"человек", 0}}},
                                                {"человек", {{"машина", 0}, {"человек", 0}}}};
    QTest::newRow("twoIndependentClasses") << twoIndependentClasses << QStringList {"digraph ClassHierarchy {", "машина;", "человек;", "}"};

    //Простое наследование
    newQMapName simpleInheritance = {{"человек", {{"человек", 0},{"студент", 0}}},
                                                {"студент", {{"человек", 1}, {"студент", 0}}}};
    QTest::newRow("twoIndependentClasses") << simpleInheritance << QStringList {"digraph ClassHierarchy {","человек;","студент;","студент->человек;", "}"};

    //Множественное наследование
    newQMapName multipleInheritance = {{"военный", {{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                                                {"студент", {{"военный", 0}, {"студент", 0}, {"студент_военного_училища", 0}}},
                                                {"студент_военного_училища", {{"военный", 1}, {"студент", 1}, {"студент_военного_училища", 0}}}};
    QTest::newRow("multipleInheritance") << multipleInheritance << QStringList {"digraph ClassHierarchy {","военный;","студент;","студент_военного_училища;",
                                                                               "студент_военного_училища->студент;","студент_военного_училища->военный;", "}"};

    //Глубокая иерархия
    newQMapName deepHierarchy = {{"животные", {{"животные", 0},{"млекопитающие", 0},{"хищные", 0}, {"собака", 0}}},
                                                {"млекопитающие", {{"животные", 1},{"млекопитающие", 0},{"хищные", 0}, {"собака", 0}}},
                                                {"хищные", {{"животные", 1},{"млекопитающие", 1}, {"хищные", 0}, {"собака", 0}}},
                                                {"собака", {{"животные", 0},{"млекопитающие", 0}, {"хищные", 1}, {"собака", 0}}}};
    QTest::newRow("deepHierarchy") << deepHierarchy << QStringList {"digraph ClassHierarchy {","животные;","млекопитающие;","хищные;","собака;",
                                                                         "млекопитающие->животные;","хищные->млекопитающие;","собака->хищные;", "}"};

    //Ромбовидное наследование
    newQMapName rhomboidInheritance = {{"животные", {{"животные", 0},{"млекопитающие", 0},{"птицы", 0}, {"летучая_мышь", 0}}},
                                                {"млекопитающие", {{"животные", 1},{"млекопитающие", 0},{"птицы", 0}, {"летучая_мышь", 0}}},
                                                {"птицы", {{"животные", 1},{"млекопитающие", 0}, {"птицы", 0}, {"летучая_мышь", 0}}},
                                                {"летучая_мышь", {{"животные", 0},{"млекопитающие", 1}, {"птицы", 1}, {"летучая_мышь", 0}}}};
    QTest::newRow("rhomboidInheritance") << rhomboidInheritance << QStringList {"digraph ClassHierarchy {","животные;","млекопитающие;","птицы;","летучая_мышь;",
                                                                               "млекопитающие->животные;","птицы->животные;","летучая_мышь->птицы;",
                                                                               "летучая_мышь->млекопитающие;", "}"};

    //Несколько иерархий классов
    newQMapName someClassHierarchies = {{"транспорт", {{"транспорт", 0},{"хищники", 0},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"хищники", {{"транспорт", 0},{"хищники", 0},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"лев", {{"транспорт", 0},{"хищники", 1},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"троллейбус", {{"транспорт", 1},{"хищники", 0},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"сова", {{"транспорт", 0},{"хищники", 1},{"лев", 0},{"троллейбус", 0},{"сова", 0}}}};
    QTest::newRow("someClassHierarchies") << someClassHierarchies << QStringList {"digraph ClassHierarchy {","транспорт;","хищники;","лев;","троллейбус;",
                                                                                 "сова;","троллейбус->транспорт;","лев->хищники;","сова->хищники;", "}"};
}


void tests_classHierarchyDOT::tests()
{
    QFETCH(newQMapName, matrix);
    QFETCH(QStringList, expResult);

    QString result = classHierarchyDOT(matrix);

    for (const QString& part: expResult)
    {
        QVERIFY2(result.contains(part), qPrintable(QString("Result doesn't contain expected part %1").arg(part)));
    }
}
