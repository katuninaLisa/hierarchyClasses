#include "testsbuildhierarchy.h"
#include <QtTest>

testsBuildHierarchy::testsBuildHierarchy(QObject *parent) : QObject(parent)
{}

//Вспомогательный метод
QSet<Property> testsBuildHierarchy::createProperties(const QMap <QString, QVector<int>> &prop)
{
    QSet <Property> properties;
    for (auto it1 = prop.begin(); it1 != prop.end(); ++it1)
    {
        properties.insert(Property(it1.key(), it1.value()));
    }
    return properties;
}

void testsBuildHierarchy::testBuildHierarchy_data()
{
    //Создаем колонки-параметры метода
    QTest::addColumn<QSet<Class>>("classes");
    QTest::addColumn<newQMapName>("expMatrix");

    //Создаем строки-тесты
    // Пустой список классов
    QTest::newRow("emptyClasses") << QSet <Class> () << newQMapName ();

    // Один класс без наследников
    QSet <Class> singleClass;
    singleClass.insert(Class("человек"));
    newQMapName singleMatrix;
    singleMatrix["человек"] = {{"человек", 0}};

    QTest::newRow("singleClass") << singleClass << singleMatrix;

    // Два независимых класса
    QSet <Class> twoClasses;
    Property humanProp("фио");
    Property animalProp("вид");
    twoClasses.insert(Class("человек", humanProp));
    twoClasses.insert(Class("животное", animalProp));

    newQMapName twoMatrix;
    twoMatrix = {{"человек", {{"человек", 0},{"животное", 0}}},
                 {"животное", {{"человек", 0}, {"животное", 0}}}};

    QTest::newRow("twoIndependentClasses") << twoClasses << twoMatrix;

    // Один класс наследует другой
    QSet <Class> simpleInheritance;
    simpleInheritance.insert(Class("человек", humanProp));
    Class student2("студент");
    student2.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    simpleInheritance.insert(student2);

    newQMapName simpleInheritanceMatrix;
    simpleInheritanceMatrix = {{"человек", {{"человек", 0},{"студент", 0}}},
                 {"студент", {{"человек", 1}, {"студент", 0}}}};

    QTest::newRow("simpleInheritance") << simpleInheritance << simpleInheritanceMatrix;

    // Цепочка наследования
    QSet <Class> chainInheritance;
    chainInheritance.insert(Class("человек", humanProp));
    Class student1("студент");
    student1.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    chainInheritance.insert(student1);

    Class nadezkin("надежкин");
    nadezkin.setproperties(createProperties({{"фио", {}}, {"курс", {2}}}));
    chainInheritance.insert(nadezkin);

    newQMapName chainInheritanceMatrix;
    chainInheritanceMatrix = {{"человек", {{"человек", 0},{"студент", 0}, {"надежкин", 0}}},
                 {"студент", {{"человек", 1}, {"студент", 0}, {"надежкин", 0}}},
                 {"надежкин", {{"человек", 0}, {"студент", 1}, {"надежкин", 0}}}};

    QTest::newRow("chainInheritance") << chainInheritance << chainInheritanceMatrix;

    // Множественное наследования
    QSet <Class> multipleInheritance;
    Class military1("военный");
    military1.setproperties(createProperties({{"фио", {}}, {"звание", {}}}));
    multipleInheritance.insert(military1);

    Class student3("студент");
    student3.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    multipleInheritance.insert(student3);

    Class student_military1("студент_военного_училища");
    student_military1.setproperties(createProperties({{"фио", {}}, {"курс", {}}, {"звание", {}}}));
    multipleInheritance.insert(student_military1);

    newQMapName multipleInheritanceMatrix;
    multipleInheritanceMatrix = {{"военный", {{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент", {{"военный", 0}, {"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент_военного_училища", {{"военный", 1}, {"студент", 1}, {"студент_военного_училища", 0}}}};

    QTest::newRow("multipleInheritance") << multipleInheritance << multipleInheritanceMatrix;

    // Несколько уровней наследования
    QSet <Class> multiLevelInheritance;
    multiLevelInheritance.insert(Class("человек", humanProp));

    Class military2("военный");
    military2.setproperties(createProperties({{"фио", {}}, {"звание", {}}}));
    multiLevelInheritance.insert(military2);

    Class student4("студент");
    student4.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    multiLevelInheritance.insert(student4);

    Class student_military2("студент_военного_училища");
    student_military2.setproperties(createProperties({{"фио", {}}, {"курс", {}}, {"звание", {}}}));
    multiLevelInheritance.insert(student_military2);

    newQMapName multiLevelInheritanceMatrix;
    multiLevelInheritanceMatrix = {{"человек", {{"человек", 0},{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"военный", {{"человек", 1},{"военный", 0},{"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент", {{"человек", 1},{"военный", 0}, {"студент", 0}, {"студент_военного_училища", 0}}},
                 {"студент_военного_училища", {{"человек", 0},{"военный", 1}, {"студент", 1}, {"студент_военного_училища", 0}}}};

    QTest::newRow("multiLevelInheritance") << multiLevelInheritance << multiLevelInheritanceMatrix;

    // Несколько иерархий классов
    QSet <Class> multipleHierarchies;
    multipleHierarchies.insert(Class("человек", humanProp));

    Property carProp("способ_передвижения");
    multipleHierarchies.insert(Class("транспорт", carProp));

    Class military3("военный");
    military3.setproperties(createProperties({{"фио", {}}, {"звание", {}}}));
    multipleHierarchies.insert(military3);

    Class student5("студент");
    student5.setproperties(createProperties({{"фио", {}}, {"курс", {}}}));
    multipleHierarchies.insert(student5);

    Class trolleybus("троллейбус");
    trolleybus.setproperties(createProperties({{"способ_передвижения", {}}, {"билет", {}}}));
    multipleHierarchies.insert(trolleybus);

    newQMapName multipleHierarchiesMatrix;
    multipleHierarchiesMatrix = {{"транспорт", {{"транспорт", 0},{"человек", 0},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"человек", {{"транспорт", 0},{"человек", 0},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"военный", {{"транспорт", 0},{"человек", 1},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"студент", {{"транспорт", 0},{"человек", 1},{"военный", 0},{"студент", 0},{"троллейбус", 0}}},
                 {"троллейбус", {{"транспорт", 1},{"человек", 0},{"военный", 0},{"студент", 0},{"троллейбус", 0}}}};

    QTest::newRow("multipleHierarchies") << multipleHierarchies << multipleHierarchiesMatrix;
}

void testsBuildHierarchy::testBuildHierarchy()
{
    QFETCH(QSet<Class>, classes);
    QFETCH(newQMapName, expMatrix);

    newQMapName matrix;
    buildHierarchy(classes, matrix);

    QCOMPARE(matrix, expMatrix);
}
