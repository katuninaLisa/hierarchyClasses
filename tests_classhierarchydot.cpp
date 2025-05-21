#include "tests_classhierarchydot.h"
#include <QtTest>

tests_classHierarchyDOT::tests_classHierarchyDOT(QObject *parent) : QObject(parent) {}

void tests_classHierarchyDOT::emptyMatrix()
{
    QMap <QString, QMap <QString, int>> matrix;

    QString result = classHierarchyDOT(matrix);

    QString exp_result = "digraph ClassHierarchy {"
                         "}";

    QCOMPARE(result, exp_result);
}

void tests_classHierarchyDOT::singleClass()
{
    QMap <QString, QMap <QString, int>> matrix;
    matrix["животное"] = {{"животное", 0}};

    QString result = classHierarchyDOT(matrix);

    QString exp_result = "digraph ClassHierarchy {"
                         "животное;"
                         "}";

    QCOMPARE(result, exp_result);
}

void tests_classHierarchyDOT::twoIndependentClasses()
{
    QMap <QString, QMap <QString, int>> matrix ={{"машина", {{"машина", 0},{"человек", 0}}},
                {"человек", {{"машина", 0}, {"человек", 0}}}};

    QString result = classHierarchyDOT(matrix);

    QStringList expectedResult = {"digraph ClassHierarchy {",
                                  "машина;",
                                  "человек;",
                                  "}"};

    for (QString part: expectedResult)
    {
        QVERIFY(result.contains(part));
    }
}

void tests_classHierarchyDOT::simpleInheritance()
{
    QMap <QString, QMap <QString, int>> matrix = {{"человек", {{"человек", 0},{"студент", 0}}},
                 {"студент", {{"человек", 1}, {"студент", 0}}}};

    QString result = classHierarchyDOT(matrix);

    QStringList expectedResult = {"digraph ClassHierarchy {",
                             "человек;",
                             "студент;",
                             "студент->человек;",
                             "}"};

    for (QString part: expectedResult)
    {
        QVERIFY(result.contains(part));
    }
}

void tests_classHierarchyDOT::multipleInheritance()
{
    QMap <QString, QMap <QString, int>> matrix = {{"военный", {{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент", {{"военный", 0}, {"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент_военного_училища", {{"военный", 1}, {"студент", 1}, {"студент_военного_училища", 0}}}};

    QString result = classHierarchyDOT(matrix);

    QStringList expectedResult = {"digraph ClassHierarchy {",
                                  "военный;",
                                  "студент;",
                                  "студент_военного_училища;",
                                  "студент_военного_училища->студент;",
                                  "студент_военного_училища->военный;",
                                  "}"};

    for (QString part: expectedResult)
    {
        QVERIFY(result.contains(part));
    }
}

void tests_classHierarchyDOT::deepHierarchy()
{
    QMap <QString, QMap <QString, int>> matrix = {{"животные", {{"животные", 0},{"млекопитающие", 0},{"хищные", 0}, {"собака", 0}}},
                                                {"млекопитающие", {{"животные", 1},{"млекопитающие", 0},{"хищные", 0}, {"собака", 0}}},
                                                {"хищные", {{"животные", 1},{"млекопитающие", 1}, {"хищные", 0}, {"собака", 0}}},
                                                {"собака", {{"животные", 0},{"млекопитающие", 0}, {"хищные", 1}, {"собака", 0}}}};

    QString result = classHierarchyDOT(matrix);

    QStringList expectedResult = {"digraph ClassHierarchy {",
                                  "животные;",
                                  "млекопитающие;",
                                  "хищные;",
                                  "собака;",
                                  "млекопитающие->животные;",
                                  "хищные->млекопитающие;",
                                  "собака->хищные;",
                                  "}"};

    for (QString part: expectedResult)
    {
        QVERIFY(result.contains(part));
    }
}

void tests_classHierarchyDOT::rhomboidInheritance()
{
    QMap <QString, QMap <QString, int>> matrix = {{"животные", {{"животные", 0},{"млекопитающие", 0},{"птицы", 0}, {"летучая_мышь", 0}}},
                                                {"млекопитающие", {{"животные", 1},{"млекопитающие", 0},{"птицы", 0}, {"летучая_мышь", 0}}},
                                                {"птицы", {{"животные", 1},{"млекопитающие", 0}, {"птицы", 0}, {"летучая_мышь", 0}}},
                                                {"летучая_мышь", {{"животные", 0},{"млекопитающие", 1}, {"птицы", 1}, {"летучая_мышь", 0}}}};

    QString result = classHierarchyDOT(matrix);

    QStringList expectedResult = {"digraph ClassHierarchy {",
                                  "животные;",
                                  "млекопитающие;",
                                  "птицы;",
                                  "летучая_мышь;",
                                  "млекопитающие->животные;",
                                  "птицы->животные;",
                                  "летучая_мышь->птицы;",
                                  "летучая_мышь->млекопитающие;",
                                  "}"};

    for (QString part: expectedResult)
    {
        QVERIFY(result.contains(part));
    }
}

void tests_classHierarchyDOT::someClassHierarchies()
{
    QMap <QString, QMap <QString, int>> matrix = {{"транспорт", {{"транспорт", 0},{"хищники", 0},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"хищники", {{"транспорт", 0},{"хищники", 0},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"лев", {{"транспорт", 0},{"хищники", 1},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"троллейбус", {{"транспорт", 1},{"хищники", 0},{"лев", 0},{"троллейбус", 0},{"сова", 0}}},
                                                {"сова", {{"транспорт", 0},{"хищники", 1},{"лев", 0},{"троллейбус", 0},{"сова", 0}}}};

    QString result = classHierarchyDOT(matrix);

    QStringList expectedResult = {"digraph ClassHierarchy {",
                                  "транспорт;",
                                  "хищники;",
                                  "лев;",
                                  "троллейбус;",
                                  "сова;",
                                  "троллейбус->транспорт;",
                                  "лев->хищники;",
                                  "сова->хищники;",
                                  "}"};

    for (QString part: expectedResult)
    {
        QVERIFY(result.contains(part));
    }
}
